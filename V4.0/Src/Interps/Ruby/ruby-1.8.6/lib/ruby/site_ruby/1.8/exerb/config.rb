
#==============================================================================#
# $Id: config.rb,v 1.22 2007/06/17 14:47:11 arton Exp $
#==============================================================================#

require 'rbconfig'

#==============================================================================#

module Exerb

  # Search directories of a core.
  # If running exerb on exerb, Add self path to the search directories of a core.
  CORE_PATH = [
    (File.dirname(ExerbRuntime.filepath) if defined?(ExerbRuntime)),
    ENV['EXERBCORE'],
    File.join(Config::CONFIG['datadir'], 'exerb'),
    '.',
  ].compact

  # Name definitions of a core.
  CORE_NAME = {
    'cui'    => 'ruby186c.exc',
    'cuid'   => 'ruby186cd.exc',
    'cuirt'  => 'ruby186crt.exc',
    'cuirtd' => 'ruby186crtd.exc',
    'gui'    => 'ruby186g.exc',
    'guid'   => 'ruby186gd.exc',
    'guirt'  => 'ruby186grt.exc',
    'guirtd' => 'ruby186grtd.exc',
    'cui19'  => 'ruby190c.exc',
    'gui19'  => 'ruby190g.exc',
    'cui20'  => 'ruby200c.exc',
    'gui20'  => 'ruby200g.exc',
  }

  # Descriptions of a core.
  CORE_DESC = {
    # FIXME: Add descriptions
    # 'ruby186c.exc' => '...',
  }

end # Exerb

#==============================================================================#

# Load a configuration file of Exerb Core Collection if found that.
configcc = File.join(File.dirname(__FILE__), 'configcc.rb')
if File.exist?(configcc)
  require(configcc)
end

#==============================================================================#
#==============================================================================#
