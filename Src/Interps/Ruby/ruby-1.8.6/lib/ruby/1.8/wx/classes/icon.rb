class Wx::Icon
  # Analogous to Image.from_bitmap
  def self.from_bitmap(bmp)
    ico = new
    ico.copy_from_bitmap(bmp)
    ico
  end

  # Redefine the initialize method so it raises an exception if a
  # non-existent file is given to the constructor; otherwise, wx Widgets
  # just carries on with an empty icon, which may cause faults later
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    if args[0].kind_of? String
      if not File.exist?( File.expand_path(args[0]) )
        Kernel.raise(ArgumentError, "Icon file does not exist: #{args[0]}")
      end
    end
    wx_init.bind(self).call(*args)
  end
end
