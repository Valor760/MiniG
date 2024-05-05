#include "layout.h"
#include "layout_common.h"

namespace MiniG::Gui
{
const static std::vector<Layout(*)()> g_LayoutFuncs = {
	GetLayout_MainMenu, GetLayout_SelectGame, GetLayout_Tetris,
	GetLayout_Snake,
};

const std::vector<void(*)()> g_LayoutInitFuncs = {
	Gui::Init_LayoutMainMenu, Gui::Init_LayoutGameSelection,
};

/* Call add all layouts to global array in constructor */
LayoutManager::LayoutManager()
{
	for(auto func : g_LayoutInitFuncs)
	{
		func();
	}

	/* Init with Main Menu layout */
	m_CurrentLayout = GetLayout_MainMenu();
}

/* TODO: Should all 'draw' functions be moved to separate file/renderer class or stay here? */
static bool draw_button(const Button* button)
{
	if(button->Size.x <= 0 || button->Size.y <= 0)
	{
		return false;
	}

	bool button_pressed = false;

	if(button->IsShadow)
	{
		/* Hardcoded text shadow */
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.55f, 1.0f));
	}

	ImGui::SetCursorPos(button->Position);
	if(ImGui::Button(button->Label.c_str(), button->Size))
	{
		if(button->pButtonPressedCallback)
		{
			button->pButtonPressedCallback(button->CallbackArgs);
			button_pressed = true;
		}
	}

	if(button->IsShadow)
	{
		ImGui::PopStyleColor();
	}

	return button_pressed;
}

static inline void LayoutPushStyle(LayoutWindow* window)
{
	for(const auto& s : window->Style)
	{
		if(s.Type == StyleVarType::Float)
		{
			ImGui::PushStyleVar(s.Var, std::get<float>(s.Value));
		}
		else /* StyleVarType::Vec2 */
		{
			ImGui::PushStyleVar(s.Var, std::get<ImVec2>(s.Value));
		}
	}

	for(const auto& c : window->Color)
	{
		ImGui::PushStyleColor(c.Type, c.Color);
	}

	if(window->Font)
	{
		ImGui::PushFont(window->Font);
	}
}

static inline void LayoutPopStyle(LayoutWindow* window)
{
	if(window->Font)
	{
		ImGui::PopFont();
	}
	ImGui::PopStyleColor(window->Color.size());
	ImGui::PopStyleVar(window->Style.size());
}

bool LayoutManager::DrawLayoutImpl()
{
	/* TODO: Do we need to store layout in pointers? */
	for(auto* window : m_CurrentLayout.WindowStack)
	{
		ImGui::SetNextWindowPos(window->Position);
		ImGui::SetNextWindowSize(window->Size);
		ImGui::Begin(window->Label.c_str(), nullptr, window->Flags);

		LayoutPushStyle(window);

		/* Draw background */
		if(window->Background.IsReady())
		{
			ImGui::SetCursorPos({0, 0});
			ImGui::Image((void*)(int64_t)window->Background.GetID(), window->Size);
		}

		for(auto& item : window->Items)
		{
			switch(item.Type)
			{
				case ItemType::Button:
				{
					draw_button(std::get<Button*>(item.objItem));
					/* FIXME: How else can I track, that we need to switch layout and avoid various errors? */
					if(m_NeedSwitchLayout)
					{
						m_NeedSwitchLayout = false;
						LayoutPopStyle(window);
						ImGui::End();
						goto Exit;
					}
					break;
				}
				default:
				{
					LOG_DEBUG("Unknown item type received for rendering [%d]", item.Type);
					break;
				}
			}
		}
		LayoutPopStyle(window);
		ImGui::End();
	}

Exit:
	return true;
}

BUTTON_CALLBACK_FUNC(LayoutManager::SwitchLayoutImpl)
{
	LOG_DEBUG("");

	if(args.size() != 1)
	{
		LOG_ERROR("Wrong number of arguments provided. Expected[%d], but got[%d]", 1, args.size());
		return;
	}

	const std::string search_name = args[0];
	bool found_layout = false;
	for(auto* layout_func : g_LayoutFuncs)
	{
		Layout l = layout_func();
		if(l.Name == search_name)
		{
			found_layout = true;
			m_CurrentLayout = l;
			m_NeedSwitchLayout = true;
			LOG_DEBUG("Switched layout to %s", m_CurrentLayout.Name.c_str());
			break;
		}
	}

	if(!found_layout)
	{
		LOG_ERROR("Failed to find layout - %s", search_name.c_str());
	}
}

} /* namespace MiniG::Gui */