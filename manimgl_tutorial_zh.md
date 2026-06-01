# ManimGL 中文教程

ManimGL 是 3Blue1Brown 官方出品的数学动画引擎，本教程将帮助您快速上手并掌握这个强大的工具。

## 目录

1. [安装指南](#1-安装指南)
2. [基本概念](#2-基本概念)
3. [快速入门](#3-快速入门)
4. [Mobject 详解](#4-mobject-详解)
5. [动画系统](#5-动画系统)
6. [场景管理](#6-场景管理)
7. [进阶技巧](#7-进阶技巧)
8. [常见问题](#8-常见问题)

---

## 1. 安装指南

### 系统要求

- Python 3.7 或更高版本
- FFmpeg（视频编码）
- OpenGL（图形渲染）
- LaTeX（可选，用于数学公式）

### 安装步骤

```bash
# 使用 pip 安装
pip install manimgl

# 验证安装
manimgl
```

### 从源码安装

```bash
git clone https://github.com/3b1b/manim.git
cd manim
pip install -e .
```

### Linux 额外依赖

```bash
# Debian/Ubuntu
sudo apt-get install libcairo2-dev libpango1.0-dev libgirepository1.0-dev

# Fedora
sudo dnf install cairo-devel pango-devel gobject-introspection-devel
```

---

## 2. 基本概念

### 2.1 三大核心概念

ManimGL 有三个核心概念：

| 概念 | 说明 | 类名 |
|------|------|------|
| **Mobject** | 所有可视化对象的基类 | `Mobject` |
| **Animation** | 动画效果 | `Animation` |
| **Scene** | 场景容器 | `Scene` |

### 2.2 对象层级

```
Scene
└── Mobject
    ├── VMobject (矢量图形)
    │   ├── Circle
    │   ├── Square
    │   ├── Line
    │   └── Polygon
    ├── Text
    ├── Tex
    ├── ImageMobject
    └── ThreeDObject
```

---

## 3. 快速入门

### 3.1 最小示例

```python
from manimlib import *

class HelloWorld(Scene):
    def construct(self):
        text = Text("Hello World!")
        self.add(text)
        self.wait()
```

运行命令：
```bash
manimgl your_file.py HelloWorld
```

### 3.2 添加动画

```python
from manimlib import *

class AnimatedSquare(Scene):
    def construct(self):
        square = Square()
        self.play(ShowCreation(square))
        self.wait()
```

### 3.3 常用命令

```bash
# 实时预览
manimgl example.py SceneName

# 导出视频
manimgl -w example.py SceneName

# 导出并打开
manimgl -o example.py SceneName

# 仅显示最终帧
manimgl -s example.py SceneName

# 跳到第 n 个动画
manimgl -n 5 example.py SceneName
```

---

## 4. Mobject 详解

### 4.1 基础几何图形

```python
# 圆形
circle = Circle()

# 正方形
square = Square()

# 矩形
rectangle = Rectangle(width=4, height=2)

# 三角形
triangle = Triangle()

# 线条
line = Line(start=LEFT, end=RIGHT)

# 带箭头的线
arrow = Arrow(start=LEFT, end=RIGHT)

# 多边形
polygon = RegularPolygon(n=6)  # 六边形
```

### 4.2 文字与公式

```python
# 普通文本
text = Text("Hello")

# LaTeX 公式
formula = Tex(r"\frac{d}{dx}(f(x))")

# 多行公式
multiline = Tex(r"""
    E &= mc^2 \\
    F &= ma
""")
```

### 4.3 颜色设置

```python
# 使用颜色常数
square = Square().set_fill(BLUE, opacity=0.5)
square.set_stroke(RED, width=2)

# 使用十六进制颜色
square.set_fill("#FF5733")

# 渐变色
square.set_fill_by_gradient(BLUE, RED)

# 常用颜色常数
RED, BLUE, GREEN, YELLOW, ORANGE, PURPLE, PINK, WHITE, BLACK, GRAY
```

### 4.4 位置与变换

```python
# 位置
square.to_edge(LEFT)
square.to_corner(UR)  # 右上角
square.center()

# 移动
square.shift(UP)
square.shift(2 * RIGHT)
square.move_to(ORIGIN)

# 旋转
square.rotate(PI/4)
square.rotate(45 * DEGREES)

# 缩放
square.scale(2)
square.scale(0.5, about_point=ORIGIN)

# 环绕
circle.surround(square)
```

### 4.5 坐标系统

```python
# 使用坐标
point = np.array([1, 2, 0])

# 常数坐标
ORIGIN      # (0, 0, 0)
UP          # (0, 1, 0)
DOWN        # (0, -1, 0)
LEFT        # (-1, 0, 0)
RIGHT       # (1, 0, 0)
UL          # 左上
UR          # 右上
DL          # 左下
DR          # 右下

# 坐标轴
axes = Axes(x_range=[-3, 3], y_range=[-2, 2])
graph = axes.get_graph(lambda x: x**2)
```

---

## 5. 动画系统

### 5.1 创建动画

```python
# 显示创建
self.play(ShowCreation(circle))

# FadeIn/FadeOut
self.play(FadeIn(circle))
self.play(FadeOut(circle))

# Write (写字效果)
self.play(Write(text))

# DrawBorderThenFill (描边后填充)
self.play(DrawBorderThenFill(square))
```

### 5.2 变换动画

```python
# 旋转变换
self.play(Rotate(square, PI))

# 缩放变换
self.play(ScaleInPlace(square, 2))

# 形状变换
self.play(Transform(circle, square))

# 精确替换变换
self.play(TransformMatchingTex(formula1, formula2))
```

### 5.3 移动动画

```python
# 移动到目标
self.play(MoveToTarget(circle))

# 路径动画
self.play(MoveAlongPath(ball, path))

# 保持物件跟随
def update_func(mob, dt):
    mob.shift(UP * dt)
square.add_updater(update_func)
```

### 5.4 组合动画

```python
# 顺序播放
self.play(Animation1())
self.play(Animation2())

# 同时播放
self.play(Animation1(), Animation2())

# 交错播放
self.play(Animation1(), Animation2(), Animation3())

# 延迟播放
self.play(Animation1(), rate_func=there_and_back, run_time=2)
```

### 5.5 速率函数

```python
from manimlib import *

# 线性
linear

# 平滑
smooth
there_and_back

# 缓动
rush_into
rush_from
slow_into

# 震荡
running_start
not_quite_there
wiggle

# 弹性
elastic
bounce_end
```

---

## 6. 场景管理

### 6.1 场景基础

```python
class MyScene(Scene):
    def construct(self):
        # 初始化场景
        pass
    
    def setup(self):
        # 在 construct 之前执行
        pass
    
    def tear_down(self):
        # 在场景结束时执行
        pass
```

### 6.2 场景组件

```python
def construct(self):
    # 添加对象
    self.add(square)
    
    # 移除对象
    self.remove(circle)
    
    # 播放动画
    self.play(animation)
    
    # 等待
    self.wait(duration=2)
    
    # 清屏
    self.clear()
    
    # 恢复默认相机
    self.restructure(True)
```

### 6.3 相机控制

```python
# 相机平移
self.play(self.camera_frame.shift, UP)

# 相机缩放
self.play(self.camera_frame.scale, 0.5)

# 相机移动
self.play(self.camera_frame.move_to, [3, 2, 0])

# 设置相机速度
self.camera_frame.set_move_along_path(path, 2)
```

### 6.4 交互式场景

```python
from manimlib import *

class InteractiveScene(Scene):
    def construct(self):
        circle = Circle()
        self.add(circle)
        
        self.interactive_embed()
```

---

## 7. 进阶技巧

### 7.1 自定义 Mobject

```python
class MyCustomObject(VMobject):
    def __init__(self, color=BLUE, **kwargs):
        super().__init__(**kwargs)
        self.set_points([
            [-1, -1, 0],
            [1, -1, 0],
            [1, 1, 0],
            [-1, 1, 0],
        ])
        self.set_fill(color, opacity=0.8)
        self.set_stroke(color, width=2)
```

### 7.2 分组与解组

```python
from manimlib import *

def construct(self):
    # 创建组
    group = VGroup(circle, square, triangle)
    group.scale(2)
    group.shift(UP)
    
    self.play(ShowCreation(group))
    
    # 解组
    group.submobjects[0].generate_target()
    group.submobjects[0].target.shift(DOWN)
    
    self.play(MoveToTarget(group.submobjects[0]))
```

### 7.3 路径动画

```python
from manimlib import *
import numpy as np

def construct(self):
    path = CubicBezier(
        [0, 0, 0],
        [1, 2, 0],
        [2, -1, 0],
        [3, 0, 0]
    )
    
    dot = Dot()
    self.play(MoveAlongPath(dot, path))
```

### 7.4 数值追踪

```python
from manimlib import *

class ValueTrackerDemo(Scene):
    def construct(self):
        number = DecimalNumber(0)
        self.add(number)
        
        number.add_updater(lambda m: m.set_value(self.time))
        self.wait(3)
```

### 7.5 布尔运算

```python
from manimlib import *

def construct(self):
    circle = Circle()
    square = Square()
    
    # 并集
    union = Union(circle, square)
    
    # 交集
    intersection = Intersection(circle, square)
    
    # 差集
    difference = Difference(circle, square)
    
    # 异或
    xor = Exclusion(circle, square)
    
    self.play(ShowCreation(union))
```

### 7.6 3D 图形

```python
from manimlib import *

class ThreeDDemo(Scene):
    def construct(self):
        self.set_camera_orientation(phi=75 * DEGREES, theta=-45 * DEGREES)
        
        axes = ThreeDAxes()
        sphere = Sphere()
        
        self.play(ShowCreation(axes))
        self.play(ShowCreation(sphere))
        self.begin_ambient_camera_rotation(rate=0.3)
        self.wait(3)
        self.stop_ambient_camera_rotation()
```

---

## 8. 常见问题

### Q1: 如何导出透明背景的视频？

```bash
manimgl -t your_file.py SceneName
```

### Q2: 如何设置输出分辨率？

```python
# 在 custom_config.yml 中设置
output:
  pixel_height: 1080
  pixel_width: 1920
```

### Q3: 如何处理 LaTeX 错误？

确保已安装完整的 LaTeX 发行版（如 TeX Live 或 MiKTeX）。常见错误：
- 缺少包：`sudo apt-get install texlive-latex-extra`
- 编译超时：增加 `preview_latex_timeout` 配置

### Q4: 如何实现循环动画？

```python
def construct(self):
    square = Square()
    self.add(square)
    
    square.generate_target()
    
    while True:
        square.target.shift(RIGHT)
        self.play(MoveToTarget(square), run_time=1)
        square.generate_target()
        square.target.shift(LEFT)
        self.play(MoveToTarget(square), run_time=1)
        square.generate_target()
```

### Q5: 如何调整动画速度？

```python
# 全局速度
self.play(animation, run_time=2)  # 2秒

# 速率函数
self.play(animation, rate_func=smooth)

# 速率和时间的组合
self.play(animation, rate_func=rush_into, run_time=1.5)
```

---

## 附录 A: 常用配置

### custom_config.yml 示例

```yaml
quality:
  pixel_height: 1080
  pixel_width: 1920
  frame_rate: 60

output:
  video_directory: ./videos
  tex_dir: ./tex_temp
  text_dir: ./text_temp
  log_dir: ./logs

style:
  font: "Sans"
  background_color: BLACK
  foreground_color: WHITE

preview:
  enable_preview: true
  live_preview: true
```

---

## 附录 B: 命令行参数

| 参数 | 说明 | 示例 |
|------|------|------|
| `-w` | 导出视频 | `manimgl -w scene.py MyScene` |
| `-o` | 导出并打开 | `manimgl -o scene.py MyScene` |
| `-s` | 显示最终帧 | `manimgl -s scene.py MyScene` |
| `-so` | 保存最终帧图片 | `manimgl -so scene.py MyScene` |
| `-n` | 跳到第 n 个动画 | `manimgl -n 5 scene.py MyScene` |
| `-f` | 全屏播放 | `manimgl -f scene.py MyScene` |
| `-t` | 透明背景 | `manimgl -t scene.py MyScene` |
| `-l` | 低质量（快速预览） | `manimgl -l scene.py MyScene` |
| `-m` | 中等质量 | `manimgl -m scene.py MyScene` |

---

## 附录 C: 参考资源

- [官方文档](https://3b1b.github.io/manim/)
- [3Blue1Brown 视频仓库](https://github.com/3b1b/videos)
- [Manim 社区版](https://github.com/ManimCommunity/manim/)
- [中文文档](https://docs.manim.org.cn/)

---

*本教程基于 ManimGL 最新版本编写，如有疏漏欢迎指正。*
