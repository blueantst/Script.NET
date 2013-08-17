# Access OS-standard locations for user files, config files etc
class Wx::StandardPaths
  class << self
    # Expose instance methods as class methods in Ruby
    Wx::StandardPaths.instance_methods(false).each do | meth |
      define_method(meth) { get.send(meth) }
    end
  end
end
