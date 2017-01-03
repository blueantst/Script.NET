class Wx::Image
  # Load a new image from an IO-like object that supports "read"
  def self.read(an_io, type_or_mime, index = -1)
    img = new
    img.load_stream(an_io, type_or_mime, index)
    img
  end

  # Create a new image from Wx::Bitmap, preserving mask information
  def self.from_bitmap(bmp)
    bmp.convert_to_image
  end

  # Redefine the initialize method so it raises an exception if a
  # non-existent file is given to the constructor; otherwise, wx Widgets
  # just carries on with an invalid image, which may cause faults later
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    if args[0].kind_of? String
      if not File.exist?( File.expand_path(args[0]) )
        Kernel.raise(ArgumentError, "Image file does not exist: #{args[0]}")
      end
    end
    wx_init.bind(self).call(*args)
  end
  
  # Convert to bitmap
  def convert_to_bitmap
    Wx::Bitmap.from_image(self)
  end
end
