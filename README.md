# 计算机科学与技术导论大作业

## 一、 前言

​		本次大作业用C++框架Qt进行窗口化应用程序的展示。构建了一个画布，能对数据结构——图进行一系列的操作，并通过图的这种数据结构对课程关系的拓扑图、学科资料、专业认识与学业规划等内容继续进行动态的展示。并且结合些许建议的HTML、CSS样式、QSS样式进行修饰。

## 二、 环境

> Qt Creator 4.13.1 (Community)
>
> Qt 5.12.10 (MinGW 7.3.0 64-bit)
>
> Visual Studio Code

## 三、 工程介绍

### 1. 主界面

​		小清新配色界面、一定透明度、圆角边框：

<img  src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/7.png" width="90%">

### 2. 进入动画

​		进入应用程序后，会有动画自动绘制“HELLO WORLD”字样的图：

<img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/8.png" width="90%">

### 3. 数据结构之图的操作

- 创建结点

  - 双击画布创建结点

- 删除结点

  - 右键单击删除结点

- 创建边

  - 左键双击几点开始扩展边，点击终点结点创建边

- 删除边

  - 删除结点后自动删除对应边

- 展示结点内容

  -  方式一：左键单击结点选中，右侧显示结点名称与内容：

    <img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/2.png" width="40%">

  -  方式二：鼠标悬浮在结点上，显示结点信息浮框:

    <img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/3.png" width="40%">

### 4. 课程拓扑关系图展示



​		以动画的方式展示了拓扑图的绘制过程，并且每一拓扑图都有对应的课程详细介绍：

<img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/4.png" width="70%">

### 5. 其他内容展示

​		同样以动画的方式创建其他内容展示的图：

<img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/5.png" width="60%">

​	点击对应结点，可以展示对应具体内容

<img src="C:/Users/yyfz/Desktop/%E5%AF%BC%E8%AE%BA%E4%BD%9C%E4%B8%9A/6.png" width="50%">

### 6. 其他细节

#### （1） 鼠标悬浮退出动画

#### （2） 光标形状变化

#### （3） 画布拖拽

#### （4） 结点拖拽

#### （5） 阴影效果