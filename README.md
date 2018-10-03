# QWsProc

## 项目发起原因
WsProcessor项目移植，其Qt分支
原项目本身存在以下缺陷：
1. 基于Java，本身运行缓慢，打包体积庞大，不便于跨平台；
2. 基于Swing类库，类库本身设计陈旧bug众多，官方已经放弃维护；
3. 自绘组件众多，完全抛弃了系统组件，风格不协调；
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
