# QtAudioWake

Native GUI utility to prevent speaker systems from sleeping. It works by playing an ultra-high frequency sine wave at a specified interval.

## Configuration
The frequency is set to 22k Hz by default. You may specify a different default [at compile time](#changing-the-default-frequency) or otherwise change it by supplying a `--frequency` argument to the application.
For example:

```bash
# Sets the frequency to 24k Hz
./QtAudioWake -f 24000
```

If you wish to start the application minimized to the system tray you can supply a `--minimized` flag.

```bash
./QtAudioWake -m
```

For available parameters use `--help`.

There is also a configuration file available under one of following paths:

- **Linux:** `$HOME/.config/crenteriae/QtAudioWake.conf`
- **MacOS:** `$HOME/Library/Preferences/com.crenteriae.QtAudioWake.plist`
- **Windows**: `HKEY_CURRENT_USER\Software\crenteriae\QtAudioWake`

## License

This project is licensed under the GPL-2.0 License - see the [LICENSE](LICENSE) file for details.

## Building

This project has a dependency on Qt6. Ensure both `qt6-base` and `qt6-multimedia`, as well as a C++ compiler and `cmake`, are installed before building.
Then, run the provided build script:

```bash
bash ./build.sh
```

This will create a `build` directory containing the application binary.

### Changing the default frequency

You may edit `build.sh` to change the default frequency by supplying a value to `-DTONE_FREQUENCY`:

```bash
# build.sh line 4
cmake -DTONE_FREQUENCY=19000 .. && cd ..
```

