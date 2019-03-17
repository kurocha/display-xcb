//
//  Window.cpp
//  This file is part of the "Display XCB" project and released under the MIT License.
//
//  Created by Samuel Williams on 16/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "Window.hpp"

#include <Input/FocusEvent.hpp>

namespace Display
{
	namespace XCB
	{
		Window::Window(Application & application, const Layout & layout) : Display::Window(application, layout), _application(application)
		{
		}
		
		Window::~Window()
		{
			close();
		}
	 
		void Window::set_title(const std::string & title)
		{
			Display::Window::set_title(title);
			
			if (_handle) {
				update_title();
			}
		}
		
		void Window::update_title()
		{
			xcb_change_property(
				_application.connection(),
				XCB_PROP_MODE_REPLACE,
				_handle,
				XCB_ATOM_WM_NAME,
				XCB_ATOM_STRING,
				8,
				_title.size(),
				_title.data()
			);
		}
		
		void Window::setup_window(bool fullscreen)
		{
			assert(!_handle);
			
			auto connection = _application.connection();
			auto screen = _application.screen();
			assert(screen);
			
			std::uint32_t value_mask, value_list[32];
			
			_handle = xcb_generate_id(connection);
			_application.insert(this);
			
			value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
			value_list[0] = screen->black_pixel;
			value_list[1] =
				XCB_EVENT_MASK_KEY_RELEASE |
				XCB_EVENT_MASK_KEY_PRESS |
				XCB_EVENT_MASK_EXPOSURE |
				XCB_EVENT_MASK_STRUCTURE_NOTIFY |
				XCB_EVENT_MASK_POINTER_MOTION |
				XCB_EVENT_MASK_BUTTON_PRESS |
				XCB_EVENT_MASK_BUTTON_RELEASE;
			
			if (fullscreen) {
				_layout.bounds.set_size_from_origin({
					screen->width_in_pixels,
					screen->height_in_pixels
				});
			}
			
			auto && size = _layout.bounds.size();
			
			xcb_create_window(connection,
				XCB_COPY_FROM_PARENT,
				_handle, screen->root,
				0, 0, size[0], size[1], 0,
				XCB_WINDOW_CLASS_INPUT_OUTPUT,
				screen->root_visual,
				value_mask, value_list);
				
			xcb_change_property(connection,
				XCB_PROP_MODE_REPLACE,
				_handle, _application.wm_protocols()->atom, XCB_ATOM_ATOM, 32, 1,
				&_application.wm_delete_window()->atom);
			
			if (fullscreen) {
				xcb_intern_atom_reply_t * wm_state = _application.intern_atom_reply("_NET_WM_STATE");
				xcb_intern_atom_reply_t * wm_fullscreen = _application.intern_atom_reply("_NET_WM_STATE_FULLSCREEN");
				
				xcb_change_property(connection,
						XCB_PROP_MODE_REPLACE,
						_handle, wm_state->atom,
						XCB_ATOM_ATOM, 32, 1,
						&wm_fullscreen->atom);
				
				free(wm_state);
				free(wm_fullscreen);
			}
			
			update_title();
		}
		
		void Window::show()
		{
			if (!_handle) {
				setup_window(_layout.fullscreen);
			}
			
			xcb_map_window(_application.connection(), _handle);
			xcb_flush(_application.connection());
		}
		
		void Window::hide()
		{
			xcb_unmap_window(_application.connection(), _handle);
			xcb_flush(_application.connection());
		}
		
		void Window::close()
		{
			if (_handle) {
				_application.remove(this);
				xcb_destroy_window(_application.connection(), _handle);
				
				_handle = 0;
			}
		}
		
		void Window::handle(xcb_client_message_event_t * event)
		{
			if (event->data.data32[0] == _application.wm_delete_window()->atom) {
				Input::FocusEvent event({}, Input::FocusEvent::CLOSED);
				
				event.apply(*this);
			}
		}
		
		void Window::handle(xcb_generic_event_t * event)
		{
			switch (event->response_type & ~0x80) {
			case XCB_CLIENT_MESSAGE:
				handle(reinterpret_cast<xcb_client_message_event_t*>(event));
				break;
			}
		}
	}
}
