//
//  Application.cpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Samuel Williams on 25/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "Application.hpp"
#include "Window.hpp"

#include <cassert>
#include <cstdlib>

#include <iostream>

namespace Display
{
	namespace XCB
	{
		Application::Application()
		{
			// Open a connection to the X server:
			_connection = xcb_connect(nullptr, nullptr);
			assert(_connection);

			// Get the first screen:
			auto setup = xcb_get_setup(_connection);
			auto iterator = xcb_setup_roots_iterator(setup);

			_screen = iterator.data;
			assert(_screen);
		}
		
		Application::~Application()
		{
			if (_connection) {
				xcb_disconnect(_connection);
				
				if (_wm_protocols)
					free(_wm_protocols);
				
				if (_wm_delete_window)
					free(_wm_delete_window);
			}
		}
		
		void Application::insert(Window * window)
		{
			auto handle = window->handle();
			
			_windows.insert({handle, window});
		}
		
		void Application::remove(Window * window)
		{
			auto handle = window->handle();
			
			_windows.erase(handle);
		}
		
		xcb_intern_atom_reply_t* Application::intern_atom_reply(const char * name, bool only_if_exists) const noexcept
		{
			xcb_intern_atom_cookie_t cookie = xcb_intern_atom(_connection, only_if_exists, strlen(name), name);
			
			xcb_generic_error_t * error = nullptr;
			auto reply = xcb_intern_atom_reply(_connection, cookie, &error);
			
			assert(!error);
			assert(reply);
			
			return reply;
		}
		
		xcb_intern_atom_reply_t * Application::wm_protocols()
		{
			if (!_wm_protocols) {
				_wm_protocols = intern_atom_reply("WM_PROTOCOLS");
			}
			
			return _wm_protocols;
		}
		
		xcb_intern_atom_reply_t * Application::wm_delete_window()
		{
			if (!_wm_delete_window) {
				_wm_delete_window = intern_atom_reply("WM_DELETE_WINDOW");
			}
			
			return _wm_delete_window;
		}
		
		void Application::run()
		{
			// Setup connnection state using setup() if required.
			// Invoke did_finish_launching()
			// Handle user events for windows. Blocking until user invokes stop().
			
			_running = true;
			
			did_finish_launching();
			
			while (_running) {
				xcb_generic_event_t * event = xcb_wait_for_event(_connection);
				
				if (event) {
					handle(event);
					free(event);
				} else {
					break;
				}
			}
		}
		
		void Application::stop()
		{
			// Possibly do some XCB related things here.
			if (will_terminate()) {
				_running = false;
			}
		}
		
		void Application::handle(xcb_client_message_event_t * event)
		{
			if (event->window) {
				auto iterator = _windows.find(event->window);
				
				if (iterator != _windows.end()) {
					iterator->second->handle(event);
				}
			}
		}
		
		void Application::handle(xcb_generic_event_t * event)
		{
			switch (event->response_type & ~0x80) {
			case XCB_CLIENT_MESSAGE:
				handle(reinterpret_cast<xcb_client_message_event_t*>(event));
				break;
			}
		}
	}
}
