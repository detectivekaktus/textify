# textify

![textify render result of README.md file inside this project](https://github.com/detectivekaktus/textify/blob/main/imgs/preview1.png)

textify is a set of tools that is easy to use and allows you to render some specific file formats, such as *markdown* (.md) and *html* (.html). It can also convert files from one format to another!

The tool is written on **Arch Linux** and is not guaranteed to work on **Windows** or any other non **UNIX-like** systems. It utilizes a lot of ANSI escape codes, so if your terminal emulator doesn't support those, it doesn't support this application either.

## How to use
Here's a list of the available commands the application has:
1. `textify [ path/to/file ]` renders the file contents. If it's a specific file format such as *markdown* or *html* it's going to apply some styling and coloring.
2. `textify -c [ path/to/source ] [ path/to/result ]` converts file into a different file format.

## To-do
Add support for displaying and converting the following file formats:
* PDF.

## Contributing
See [CONTRIBUTING.md](https://github.com/detectivekaktus/textify/blob/main/CONTRIBUTING.md) file to find more.
