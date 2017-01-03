class Wx::Clipboard
  class << self
    # Class method to provide access to the clipboard within a ruby
    # block. Tests that the clipboard could be accessed, and ensures
    # that it is closed when the block is finished.
    def open
      clip = Wx::Clipboard.new
      unless clip.open
        Kernel.raise "Could not open clipboard"
      end
      yield clip
    ensure
      clip.close
    end
  end
end
