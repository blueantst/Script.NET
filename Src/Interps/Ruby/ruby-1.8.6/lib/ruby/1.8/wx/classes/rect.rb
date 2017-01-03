# Class representing a rectangular shape
class Wx::Rect
  # Nicely readable inspect output for Rect
  def to_s
    "#<Wx::Rect: (#{get_left}, #{get_top}) (#{get_right}, #{get_bottom})>"    
  end
  
  # More ruby-ish names
  alias :contains? :contains
end
