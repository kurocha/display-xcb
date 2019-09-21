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
#include <xcb/xinput.h>

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
			
			bool xcb_input_available() const noexcept;
			
		protected:
			xcb_connection_t * _connection = nullptr;
			xcb_screen_t * _screen = nullptr;
			
			std::unordered_map<xcb_window_t, Window *> _windows;
			
			xcb_intern_atom_reply_t * _wm_protocols = nullptr;
			xcb_intern_atom_reply_t * _wm_delete_window = nullptr;
			
			// xcb_extension_t _xcb_input_id = {"XInputExtension", 0};
			const struct xcb_query_extension_reply_t * _xcb_input_reply = nullptr;
			
			void setup_extensions();
			
			void receive(xcb_generic_event_t * event);
			void receive(xcb_ge_generic_event_t * event);
			void receive(xcb_client_message_event_t * event);
			void receive(xcb_configure_notify_event_t * event);
			void receive(xcb_motion_notify_event_t * event);
			// void receive(xcb_expose_event_t * event);
			// void receive(xcb_button_press_event_t * event);
			// void receive(xcb_enter_notify_event_t * event);
			// void receive(xcb_leave_notify_event_t * event);
			// void receive(xcb_key_press_event_t * event);
			// void receive(xcb_key_release_event_t * event);
			
			void receive(xcb_input_motion_event_t * event);
			
			bool _running = false;
		};
	}
}
