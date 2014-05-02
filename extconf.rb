require 'mkmf'

extension_name = 'ReadlineExtensions'

have_library('readline', 'readline')

dir_config(extension_name)
create_makefile(extension_name)
