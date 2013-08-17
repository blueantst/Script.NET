# Class for automatically managing layouts
class Wx::Sizer
  # Generic method to add items, supporting positional and named
  # arguments
  ADD_ITEM_PARAMS = [ Wx::Parameter[ :index, -1 ], 
                      Wx::Parameter[ :proportion, 0 ],
                      Wx::Parameter[ :flag, 0 ],
                      Wx::Parameter[ :border, 0 ],
                      Wx::Parameter[ :user_data, nil ] ]
  
  def add_item(item, *mixed_args)
    args = Wx::args_as_list(ADD_ITEM_PARAMS, *mixed_args)

    # Call add to append if default position
    idx = args.shift
    if idx == -1
      add(item, *args)
    else
      insert(idx, item, *args)
    end
  end
end
