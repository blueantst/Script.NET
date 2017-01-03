# = WxSugar - Accessors
#
# The default WxRuby interface has lots and lots of methods like
#
#  * get_position()
#  * set_size(a_size)
#  * is_checked()
# 
# and so on. Methods that retrieve set, or query attributes of an object
# are more normally in Ruby called simply by the attribute name:
#
#  * position()
#  * size = a_size
#  * checked?
#
# This extension creates an alias for every WxRuby instance method that
# begins with +get_+, +set_+ or +is_+. Note that if you are calling a
# 'setter' method on self, you must explicitly send the message to self:
# 
#  # set's self size to be 100px by 100px
#  self.size = Wx::Size.new(100, 100)
#  # only sets the value of a local variable 'size'
#  size = Wx::Size.new

module WxRubyStyleAccessors
  def method_missing(sym, *args)
    case sym.to_s
    when /^(.*)\=$/
      meth = "set_#{$1}"
    when /^(.*)\?$/
      meth = "is_#{$1}"
    else
      meth = "get_#{sym}"
    end
    if respond_to?(meth)
      send(meth, *args)
    else
      e = NoMethodError.new("undefined method '#{sym}' for #{self.inspect}")
      e.set_backtrace(caller)
      Kernel.raise e
    end
  end
end

# Allow Wx-global functions to be accessed with nice syntax
module Wx
  extend WxRubyStyleAccessors
end

# Apply the syntax extensions to every class, both class methods and
# instance methods
all_classes = Wx::constants.collect { | c | Wx::const_get(c) }.grep(Class)
all_classes.each do | klass |
  klass.class_eval do
    include WxRubyStyleAccessors
    extend WxRubyStyleAccessors
  end
end
