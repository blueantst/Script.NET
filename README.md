### Script.NET介绍
Script.NET是蓝蚂蚁工作室开发的一套集成开发环境（可用于Tcl、Python、Perl、Ruby、Lua等脚本语言开发），最新的4.0版本已经在github上开源。

Script.NET是一个通用的IDE平台，编译环境是VC2008，采用类似Eclipse的组件化开发思想，由平台框架和若干组件构成，框架主要提供基础界面框架和组件的管理，其他的各种功能都由各种独立的组件实现，包括编辑器组件、控制台组件、查找组件、浏览器组件、各种脚本解释器组件等，并且定义了几种标准接口的组件（解释器组件、界面组件、工程组件），另外还提供了很多工具接口和扩展点，每个组件和工具都有自己独立的目录，不需要和其他组件有任何强依赖关系，通过写自己的扩展组件、扩展工具，可以使这个平台成为各种应用工具。

Script.NET的解释器提供了功能比较完善的脚本语言开发支持，对脚本开发方面的特性如下：
- 编辑、调试、运行tcl、python、perl、ruby、lua脚本，按照解释器组件接口开发新的解释器组件，就可以提供其他脚本或非脚本语言的支持；
- 脚本的编译/加密和生成可执行文件（每种脚本语言支持的程度不同）；
- 效率分析等辅助功能（目前只有tcl、perl脚本支持了此功能）；
- 关键字帮助，通过F1键或鼠标移动到关键字上，获取详细的帮助信息；
- 支持控制台功能（目前支持tcl/python/lua/ruby/perl控制台、telnet控制台、串口控制台可用，其他脚本控制台正在开发）；

蓝蚂蚁工作室主页：[http://www.blueantstudio.net](http://www.blueantstudio.net)

作者Blog：[http://blog.csdn.net/oceanheart](http://blog.csdn.net/oceanheart)

Script.NET使用手册网址：[http://www.blueantstudio.net/scriptnet/ssdn/index.htm](http://www.blueantstudio.net/scriptnet/ssdn/index.htm)

欢迎感兴趣的程序员参与Script.NET的优化完善，我们将会把所有做过贡献的人加入贡献者名单中。 
贡献形式包括参与开发、问题和建议反馈、完善文档等。
Script.NET 4.0编译和运行需要用到Xtreme Toolkit 13.0或以上的版本（可以从CSDN资源中搜索到，建议用比较新的15.3.1版本）。
目前在github上已经开源了平台的所有代码和Tcl等解释器的代码。

Script.NET QQ群：325880743

微信公众号：blueantstudio

### License

(The MIT License)

Copyright (c) 2000-2017 blueant studio. and other contributors

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWAR

### 一些界面截图
![主界面](http://www.blueantstudio.net/scriptnet/ssdn/operate/maingui.jpg)
![图像编辑器组件](http://www.blueantstudio.net/scriptnet/ssdn/imagetool/owmimage.jpg)
![UML组件](http://www.blueantstudio.net/scriptnet/ssdn/uml/umleditor.jpg)
![系统设置](http://www.blueantstudio.net/scriptnet/ssdn/operate/editerconf.jpg)
![系统设置](http://www.blueantstudio.net/scriptnet/ssdn/operate/outputset.jpg)
![查找](http://www.blueantstudio.net/scriptnet/ssdn/operate/output_search.jpg)
![调试](http://www.blueantstudio.net/scriptnet/ssdn/operate/debugvartip.jpg)
![控制台](http://www.blueantstudio.net/scriptnet/ssdn/terminal/term_python.jpg)

### 文档
[Script.NET编译说明.pdf](http://www.blueantstudio.net/scriptnet/opensource/Script.NET%20%E7%BC%96%E8%AF%91%E8%AF%B4%E6%98%8E.pdf)

[Script.NET调试器设计说明.pdf](http://www.blueantstudio.net/scriptnet/opensource/Script.NET%20%E8%B0%83%E8%AF%95%E5%99%A8%E8%AE%BE%E8%AE%A1%E8%AF%B4%E6%98%8E.pdf)
