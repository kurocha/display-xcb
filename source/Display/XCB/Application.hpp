//
//  Application.hpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Samuel Williams on 25/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Display/Application.hpp>

#include <xcb/xcb.h>

namespace Display
{
	namespace XCB
	{
		class Application : public Display::Application
		{
		public:
			Application();
			virtual ~Application();

			void run();
			void stop();

			auto connection() const noexcept { return _connection; }
			auto screen() const noexcept { return _screen; }
			
		protected:
			xcb_connection_t * _connection = nullptr;
			xcb_screen_t * _screen = nullptr;

			bool _running = true;
		};
	}
}
