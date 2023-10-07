#include "layout.h"
#include "layout_common.h"

namespace MiniG::Gui
{
const static std::vector<Layout(*)()> g_LayoutFuncs = {
	GetLayout_MainMenu, GetLayout_SelectGame, GetLayout_Tetris
};

/* Call add all layouts to global array in constructor */
LayoutManager::LayoutManager()
{
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

	/* TODO: Add ImGui::PushStyleColor */
	ImGui::SetCursorPos(button->Position);
	if(ImGui::Button(button->Label.c_str(), button->Size))
	{
		if(button->pButtonPressedCallback)
		{
			button->pButtonPressedCallback(button->CallbackArgs);
			return true;
		}
	}

	return false;
}

bool LayoutManager::DrawLayoutImpl()
{
	/* TODO: Do we need to store layout in pointers? */
	for(auto* window : m_CurrentLayout.WindowStack)
	{
		ImGui::SetNextWindowPos(window->Position);
		ImGui::SetNextWindowSize(window->Size);
		ImGui::Begin(window->Label.c_str(), nullptr, window->Flags);

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