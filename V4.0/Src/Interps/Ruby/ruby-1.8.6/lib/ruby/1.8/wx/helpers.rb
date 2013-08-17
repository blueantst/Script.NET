# Various non-GUI helper functions
module Wx
  # A named parameter in a Wx named-arg parameter list
  Parameter = Struct.new( :name, :default )

  # Convert mixed positional / named args into a list to be passed to
  # an underlying API method. +param_spec+ is an Array of Parameter
  # structs containing the keyword name and default value for each
  # possible argument. +mixed_args+ is an array which may optionally end
  # with a set of named arguments

  def self.args_as_list(param_spec, *mixed_args)
    # get keyword arguments from mixed args if supplied, else empty
    kwa = mixed_args.last.kind_of?(Hash) ? mixed_args.pop : {}
    out_args = []
    param_spec.each_with_index do | param, i |
      if arg = mixed_args[i] # use the supplied list arg 
        out_args << arg
      elsif kwa.key?(param.name) # use the keyword arg
        out_args << kwa[param.name]
      else # use the default argument
        out_args << param.default
      end
    end
    out_args
  rescue
    Kernel.raise ArgumentError, 
                 "Bad arg composition of #{mixed_args.inspect}"
  end
end
