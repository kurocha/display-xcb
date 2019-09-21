//
//  Window.cpp
//  This file is part of the "Display XCB" project and released under the MIT License.
//
//  Created by Samuel Williams on 16/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//


#include "Window.hpp"

#include <Input/Key.hpp>
#include <Input/FocusEvent.hpp>
#include <Input/ResizeEvent.hpp>
#include <Input/MotionEvent.hpp>

#include <Logger/Console.hpp>

#include <system_error>

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
			
			if (_application.xcb_input_available())
				setup_xcb_input();
			
			update_title();
		}
		
		void Window::setup_xcb_input()
		{
			InputEventMask input_event_mask;
			
			input_event_mask.header.deviceid = XCB_INPUT_DEVICE_ALL_MASTER;
			input_event_mask.header.mask_len = 1;
			input_event_mask.value = XCB_INPUT_XI_EVENT_MASK_MOTION;
			
			auto cookie = xcb_input_xi_select_events_checked(connection(), _handle, 1, &input_event_mask.header);
			
			auto *error = xcb_request_check(connection(), cookie);
			
			if (error) {
				throw std::runtime_error("error could not setup xinput");
			}
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
		
		void Window::receive(xcb_client_message_event_t * event)
		{
			if (event->data.data32[0] == _application.wm_delete_window()->atom) {
				Input::FocusEvent focus_event({}, Input::FocusEvent::CLOSED);
				
				focus_event.apply(*this);
			}
		}
		
		void Window::receive(xcb_configure_notify_event_t * event)
		{
			Input::ResizeEvent resize_event({}, {event->width, event->height});
			
			_layout.bounds.set_origin_and_size(
				{event->x, event->y},
				{event->width, event->height}
			);
			
			resize_event.apply(*this);
		}
		
		void Window::receive(xcb_motion_notify_event_t * event)
		{
			auto time = Time::Interval::from_milliseconds(event->time);
			const auto & bounds = _layout.bounds;
			auto key = Input::Key(Input::DefaultMouse, event->state);
			
			Input::MotionEvent motion_event(time, key, Input::State::Released, {event->event_x, event->event_y}, {0}, bounds);
			
			motion_event.apply(*this);
		}
		
		void Window::receive(xcb_input_motion_event_t * event)
		{
			auto time = Time::Interval::from_milliseconds(event->time);
			const auto & bounds = _layout.bounds;
			auto key = Input::Key(Input::DefaultMouse, 0);
			
			Input::MotionEvent motion_event(time, key, Input::State::Released, {event->event_x / 65536.0, event->event_y / 65536.0}, {0}, bounds);
			
			motion_event.apply(*this);
		}
		
		void Window::receive(xcb_generic_event_t * event)
		{
			switch (event->response_type & ~0x80) {
			case XCB_CLIENT_MESSAGE:
				receive(reinterpret_cast<xcb_client_message_event_t*>(event));
				break;
			}
		}
	}
}
