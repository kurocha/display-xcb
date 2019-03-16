//
//  Window.hpp
//  This file is part of the "Display XCB" project and released under the MIT License.
//
//  Created by Samuel Williams on 16/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Display/Window.hpp>

#include <xcb/xcb.h>

#include "Application.hpp"

namespace Display
{
	namespace XCB
	{
		class Window : public Display::Window
		{
		public:
			Window::Window(const Application & application, Layout & layout = Layout()): _layout(layout), _application(application);
			virtual ~Window();

			void set_title(const std::string & title);
			void set_cursor(Cursor cursor);
			Scale scale() const;
			
			auto handle() const noexcept { return _handle; }
			
			void show() {
				xcb_map_window(_connection, _handle);
				xcb_flush(_connection);
			}
			
			void hide() {
				xcb_unmap_window(_connection, _handle);
			}
			
		private:
			Layout & _layout = nullptr;
			Application & _application = nullptr;
			
			xcb_connection_t * _connection = nullptr;
			xcb_screen_t * _screen = nullptr;
			xcb_window_t _handle = ~0;
			
			xcb_intern_atom_reply_t * _delete_window_reply;
			
			void setup_window(bool fullscreen);
		};
	}
}
