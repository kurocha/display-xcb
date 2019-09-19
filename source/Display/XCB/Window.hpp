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
			Window(Application & application, const Layout & layout = Layout());
			virtual ~Window();

			void set_title(const std::string & title) override;
			// void set_cursor(Cursor cursor) override;
			// Scale scale() const override;
			
			auto connection() const noexcept { return _application.connection(); }
			auto handle() const noexcept { return _handle; }
			
			void show() override;
			void hide() override;
			
			void close() override;
			
			friend class Application;
			
		private:
			Application & _application;
			xcb_window_t _handle = 0;
			
			void receive(xcb_generic_event_t * event);
			void receive(xcb_client_message_event_t * event);
			void receive(xcb_configure_notify_event_t * event);
			
			void update_title();
			
			void setup_window(bool fullscreen = false);
		};
	}
}
