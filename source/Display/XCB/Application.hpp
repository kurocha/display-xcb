//
//  Application.hpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Samuel Williams on 25/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Display/Application.hpp>

#include <unordered_map>
#include <xcb/xcb.h>

namespace Display
{
	namespace XCB
	{
		class Window;
		
		class Application : public Display::Application
		{
		public:
			Application();
			virtual ~Application();
			
			void insert(Window * window);
			void remove(Window * window);
			
			xcb_intern_atom_reply_t* intern_atom_reply(const char * name, bool only_if_exists = true) const noexcept;
			xcb_intern_atom_reply_t * wm_protocols();
			xcb_intern_atom_reply_t * wm_delete_window();
			
			void run();
			void stop();

			auto connection() const noexcept { return _connection; }
			auto screen() const noexcept { return _screen; }
			
		protected:
			xcb_connection_t * _connection = nullptr;
			xcb_screen_t * _screen = nullptr;
			
			std::unordered_map<xcb_window_t, Window *> _windows;
			
			xcb_intern_atom_reply_t * _wm_protocols = nullptr;
			xcb_intern_atom_reply_t * _wm_delete_window = nullptr;
			
			void handle(xcb_generic_event_t * event);
			void handle(xcb_client_message_event_t * event);
			// void handle(xcb_expose_event_t * event);
			// void handle(xcb_button_press_event_t * event);
			// void handle(xcb_motion_notify_event_t * event);
			// void handle(xcb_enter_notify_event_t * event);
			// void handle(xcb_leave_notify_event_t * event);
			// void handle(xcb_key_press_event_t * event);
			// void handle(xcb_key_release_event_t * event);
			
			bool _running = false;
		};
	}
}
