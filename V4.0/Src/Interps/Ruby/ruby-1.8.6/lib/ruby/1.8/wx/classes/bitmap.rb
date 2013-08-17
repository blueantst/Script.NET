class Wx::Bitmap
  # Constructor copying data from an image
  def self.from_image(img, depth = -1)
    new(img, depth)
  end

  # Redefine the initialize method so it raises an exception if a
  # non-existent file is given to the constructor; otherwise, wx Widgets
  # just carries on with an empty bitmap, which may cause faults later
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    if args[0].kind_of? String
      if not File.exist?( File.expand_path(args[0]) )
        Kernel.raise(ArgumentError, "Bitmap file does not exist: #{args[0]}")
      end
    end
    wx_init.bind(self).call(*args)
  end

  # Accepts a block, which will be passed a device context which can be
  # used to draw upon the Bitmap
  def draw
    dc = Wx::MemoryDC.new
    dc.select_object(self)
    yield dc
    dc.select_object( Wx::NULL_BITMAP )
  end
end
