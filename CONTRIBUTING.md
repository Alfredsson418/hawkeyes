<h1>Contributing to this project</h1>
<p>Before contributing to this project, please read thought the <a href="CODE_OF_CONDUCT.md">Code of conduct</a> where the community rules is.</p>

<h1>Where do get help and/or join the community</h1>
<h3><a href="https://discord.gg/76USAhaF8a">Join the discord!</a></h2>
<!-- <h3><a href="">Join the Matrix server!</a> (Comming soon)</h2> -->

<h1>How to contribute</h1>
<ol>
  <li>Find something to do (<a href="#how-to-contribute">How to find someting to contribute</a>)
  <li>Fork this github repository.</li>
  <li>Create a new branch for yourself.</li>
  <li>Commit and push your changes to the forked repository.</li>
  <li>Head over to the <a href="https://github.com/Alfredsson418/hawkeyes">original</a> repository and create a pull request.</li>
  <li>Use the provided template for your pull request.</li>
  <li>Wait for it to be accepted. <b>Your pull request will only be accepted if it has a related issue</b>.</li>
  <li>Done!</li>
</ol>

<h1 id="how-to-contribute">How to find someting to contribute</h1>
<p>Head over to the <a href="https://github.com/Alfredsson418/hawkeyes/issues">Issues</a> tab or <a href="docs/FUTURE_IDEAS.md">FUTURE_IDEAS.md</a> and find something you can help with!
You could also create your own Issues with the given templates. <b>Just make sure that you get the go-ahead on the issue before creating your pull request</b>.</p>

<h1>Coding format</h1>
<p>This project is using ClangFormat to automaticlly format the source code defined in the .clang_format file.</p>

<h2>Automatic formating with ...</h2>

<h3>Zed</h3>
<p>Add this to your settings</p>

```
  "languages": {
    "C": {
      "format_on_save": "on"
    }
  },
  "file_types": {
    "C": ["c", "h"]
  }
```

<p>This configuration will automaticlly update the format on save for C, this will also determine that files ending with .c/.h will be using the C languange, with Zed sometimes gets confused with C++.</p>

<h1>Other information</h1>

<h2>Debugging</2>
<h3>GDB (debugging tool for C)</h3>
<p>If you want help with gdb, join the community for help!</p>

<h3>Listening ports on device</h3>
<p>Run `netstat -tuln` to see what ports are listening on your device when you want to troubleshoot.</p>
