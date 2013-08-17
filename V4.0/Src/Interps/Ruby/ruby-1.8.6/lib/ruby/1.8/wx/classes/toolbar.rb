# A set of buttons and controls attached to one edge of a Wx::Frame
class Wx::ToolBar
  # Generic method to add items, supporting positional and named
  # arguments
  ADD_ITEM_PARAMS = [ Wx::Parameter[ :position, -1 ], 
                      Wx::Parameter[ :id, -1 ],
                      Wx::Parameter[ :label, "" ], 
                      Wx::Parameter[ :bitmap, Wx::NULL_BITMAP ],
                      Wx::Parameter[ :bitmap2, Wx::NULL_BITMAP ],
                      Wx::Parameter[ :kind, Wx::ITEM_NORMAL ], 
                      Wx::Parameter[ :short_help, "" ], 
                      Wx::Parameter[ :long_help, "" ], 
                      Wx::Parameter[ :client_data, nil ] ]
  
  def add_item(*mixed_args)
    args = Wx::args_as_list(ADD_ITEM_PARAMS, *mixed_args)
    if args[3] == Wx::NULL_BITMAP
      Kernel.raise ArgumentError, "Main button bitmap may not be NULL"
    end

    # Call add_tool to append if default position
    pos = args.shift
    if pos == -1
      add_tool(*args)
    else
      insert_tool(pos, *args)
    end
  end
end
