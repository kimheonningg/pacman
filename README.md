### Set Up Instructions (Using Mac & VSCode)

#### How to Install and Set SDL

1. Using Homebrew, install SDL2 by:

```
brew install sdl2
```

2. At `.vscode/c_cpp_properties.json`, add the following:

```json
{
	"configurations": [
		{
			"includePath": ["path/to/SDL2"]
		}
	]
}
```

- You can check your path to SDL2 by:

```
brew info sdl2
```
