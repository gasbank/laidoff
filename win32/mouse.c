#include <stdio.h>

#include "laidoff.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	LWCONTEXT* pLwc = (LWCONTEXT*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		double normalized_xpos = 2 * (xpos / width - 0.5);
		double normalized_ypos = 1.0 - 2 * (ypos / height);

		if (action == GLFW_PRESS)
		{
			printf("glfw mouse press pos x=%f, y=%f (normalized x=%f, y=%f)\n", xpos, ypos, normalized_xpos, normalized_ypos);

			lw_trigger_touch(pLwc, (float)normalized_xpos, (float)normalized_ypos);

			lw_trigger_mouse_press(pLwc, (float)normalized_xpos, (float)normalized_ypos);
		}

		if (action == GLFW_RELEASE)
		{
			lw_trigger_mouse_release(pLwc, (float)normalized_xpos, (float)normalized_ypos);
		}
    }
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	LWCONTEXT* pLwc = (LWCONTEXT*)glfwGetWindowUserPointer(window);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double normalized_xpos = 2 * (xpos / width - 0.5);
	double normalized_ypos = 1.0 - 2 * (ypos / height);

	lw_trigger_mouse_move(pLwc, (float)normalized_xpos, (float)normalized_ypos);
}