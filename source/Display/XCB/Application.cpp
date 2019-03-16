//
//  Application.cpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Samuel Williams on 25/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "Application.hpp"

namespace Display
{
	namespace XCB
	{
		Application::Application()
		{
		}
		
		Application::~Application()
		{
			if (_connection) {
				xcb_disconnect(_connection);
			}
		}
		
		void Application::run()
		{
			// Setup connnection state using setup() if required.
			// Invoke did_finish_launching()
			// Handle user events for windows. Blocking until user invokes stop().

			// Open a connection to the X server:
			_connection = xcb_connect(nullptr, nullptr);
			
			// Get the first screen:
			auto setup = xcb_get_setup(_connection);
			auto iterator = xcb_setup_roots_iterator(setup);
			_screen = iterator.data;

			did_finish_launching();

			do {

			} while (_running);
		}
		
		void Application::stop()
		{
			// Possibly do some XCB related things here.
			if (will_terminate()) {
				_running = false;
			}
		}
	}
}
