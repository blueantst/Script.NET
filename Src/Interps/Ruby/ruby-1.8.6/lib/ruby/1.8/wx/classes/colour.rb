class Wx::Colour
  # Redefine the initialize method so it raises an exception if an
  # invalid colour value is given. This might be an unknown colour
  # string (eg 'dark blue') or out-of-bounds integer values (<0 or >255)
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    begin
      wx_init.bind(self).call(*args)
    # Invalid integer values raise SWIG 'no matching func'
    rescue ArgumentError 
      Kernel.raise ArgumentError, "Invalid colour values #{args.inspect}"
    end

    if not is_ok
      Kernel.raise ArgumentError, "Invalid colour values #{args.inspect}"
    end
  end

  # Standard colours, corresponding to WxWidgets stock colours.
  Wx::WHITE = new(255, 255, 255)
  Wx::BLACK = new(0, 0, 0)

  Wx::RED   = new(255, 0, 0)
  Wx::GREEN = new(0, 255, 0)
  Wx::BLUE  = new(0, 0, 255)

  Wx::YELLOW  = new(255, 255, 0)
  Wx::MAGENTA = new(255, 0, 255)
  Wx::CYAN    = new(0, 255, 255)

  Wx::LIGHT_GREY = new(192, 192, 192)

  # Colours are equal to one another if they have the same red, green
  # and blue intensity, and the same alpha
  def ==(other)
    if not other.is_a?(self.class)
      raise ArgumentError, "No comparison of #{self} to #{other}"
    end
    [ red, green, blue, alpha ] == 
      [ other.red, other.green, other.blue, other.alpha ]
  end

  # More informative output for inspect etc
  def to_s
    "#<Wx::Colour: (#{red}, #{green}, #{blue} *#{alpha})>"
  end
end
