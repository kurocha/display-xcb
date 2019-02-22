# Teapot v3.0.0 configuration generated at 2019-02-22 17:20:05 +1300

required_version "3.0"

define_project "vizor-platform-xcb" do |project|
	project.title = "Vizor Platform XCB"
end

# Build Targets

define_target 'vizor-platform-xcb-library' do |target|
	target.depends 'Language/C++14'
	
	target.depends 'Vulkan/Platform/XCB', public: true
	
	target.depends 'Library/Logger'
	target.depends 'Library/Vizor/Platform', public: true
	
	target.provides 'Library/Vizor/Platform/XCB' do
		source_root = target.package.path + 'source'
		
		library_path = build static_library: 'Vizor/Platform/XCB', source_files: source_root.glob('Vizor/Platform/XCB/**/*.cpp')
		
		append linkflags library_path
		append header_search_paths source_root
	end
end

define_target 'vizor-platform-xcb-test' do |target|
	target.depends 'Library/Vizor/Platform/XCB'
	target.depends 'Library/UnitTest'
	
	target.depends 'Language/C++14'
	
	target.provides 'Test/Vizor/Platform/XCB' do |arguments|
		test_root = target.package.path + 'test'
		
		run tests: 'Vizor/Platform/XCB', source_files: test_root.glob('Vizor/Platform/XCB/**/*.cpp'), arguments: arguments
	end
end

# Configurations

define_configuration 'development' do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	configuration.import "vizor-platform-xcb"
	
	# Provides all the build related infrastructure:
	configuration.require 'platforms'
	
	# Provides unit testing infrastructure and generators:
	configuration.require 'unit-test'
	
	# Provides some useful C++ generators:
	configuration.require 'generate-cpp-class'
	
	configuration.require "generate-project"
	configuration.require "vizor-platform"
end

define_configuration "vizor-platform-xcb" do |configuration|
	configuration.public!
end
