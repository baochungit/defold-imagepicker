# Defold ImagePicker
An Image Picker extension for Android, iOS.

## APIs:

* `imagepicker.show(callback_fn)`: Show the image picker and crop the selected image to square. On success, it returns the cropped image url via callback.

## Example:
```lua
if imagepicker then 
  imagepicker.show(function(_, uri)
    print("Image file:", uri)
  end)
end
```
---
