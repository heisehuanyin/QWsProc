# QWsProc

## 项目发起原因
WsProcessor项目移植，其Qt分支

原项目本身存在以下缺陷：

1. 基于Java，本身启动和运行缓慢，打包体积庞大，不便于跨平台；
2. 基于Swing类库，类库本身设计陈旧bug众多，官方已经放弃维护；
	* 超长菜单无法自动折叠；
	* TextArea输入法浮动栏不跟随光标；
	* macos上菜单栏上菜单项层次稍深便无法触发； 
3. 自绘组件众多，完全抛弃了系统组件，风格不协调；
	* JFileChooser是一个自定义的组件；
4. 非本地代码，调用本地api不方便；
5. 不同平台的表现存在差异，调试不方便；
6. 界面类库本身的组件稀少，完全依靠自定义，开发吃力；
7. 资料较少，完全依赖自身摸索，没前途；

## 项目组成
1. GUI luncher 图形界面启动器
2. Console luncher 终端启动器
3. InteractUnit 交互式宏编程解释核心
4. WsCore&&PluginManager插件管理体系
5. ContentView系列插件
6. IO_Model系列插件
7. FrontWindow系列插件
8. ConfigPort&&LogPort插件


## 大事记
2018.10.16
>监视视图机制不是一个好机制，本身架构没有设计好，存在一些问题：
>
* 无法针对整个项目进行全局的信息统计
* 无法进行单个文档的多开
* 多个视图之间无法进行互操作，也就是无法多角度的对一个文档进行修改



2018.10.8
>为了确保可以有一个多视图监视同一个打开文档方案，决定添加监视视图机制。监视视图机制可以针对任何“非视图”模块的内容进行监视，但是该监视视图中的内容保存的时候会被放弃，本机制有框架内部自动完成，组成监视链条的插件都是普通插件；
>
* 只有主编辑模块链的内容会被从顶端插件一路保存下来，监视机制模块链内容会被放弃；
* 监视机制模块链与主编辑模块链同处同一个通道，因此遵循模块不重复原则




2018.10.7
>添加了TextModel和ChannelPreface，对Configport接口进行了微小修改
>
1. 当提供写入值的时候，如果配置文件中没有配置项，则写入
2. 当未提供写入值的时候，如果配置文件没有配置项，不写入


2018.10.6
>决定对每个插件都提供一个命令行参数操作：
>
1. 要求在初始化插件工厂类后可以提取插件参数列表；
2. 在初始化插件实例过程中主框架提供初始化参数列表，可以进行定制化的插件启动；
3. 插件配置项目的持久化操作是将参数存储到主框架，并有主框架进行记录；



2018.10.4
>决定采用Qt的“Signal/Slot”观察者模式解决方案，而不采用Java的监听器解决方案，原因如下：
>
1. 监听器解决方案需要编写的代码太多，不够简洁（EventListener接口和实现、各种Event、Event的路由等）；
2. Qt自带的“Signal/Slot”机制比较好用；
3. 最关键的是在本解决方案中，同一个注册通道（由同一个注册键值引用的List集合插件集）中的插件不允许重复，也就不会发生循环引用；
