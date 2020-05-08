#include "Panel.h"

int main()
{
	Panel panel;
	panel.initData();
	while (panel.getRunning())
	{
		panel.menu();
	}
	return 0;
}
