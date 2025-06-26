#!/bin/bash

# DreamLang V2 构建脚本

set -e

echo "DreamLang V2 编译器构建脚本"
echo "=========================="

# 检查依赖
echo "检查构建依赖..."

if ! command -v cmake &> /dev/null; then
    echo "错误: 未找到 cmake。请安装 cmake 3.16 或更高版本。"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "错误: 未找到 make。请安装 make。"
    exit 1
fi

if ! command -v msgfmt &> /dev/null; then
    echo "警告: 未找到 msgfmt (gettext)。将跳过国际化文件编译。"
    SKIP_I18N=1
else
    SKIP_I18N=0
fi

# 编译国际化文件
if [ "$SKIP_I18N" -eq 0 ]; then
    echo "编译国际化文件..."
    ./compile_i18n.sh
else
    echo "跳过国际化文件编译。"
    mkdir -p share/locale/zh_CN/LC_MESSAGES
    mkdir -p share/locale/en_US/LC_MESSAGES
    # 创建空的 .mo 文件以避免运行时错误
    touch share/locale/zh_CN/LC_MESSAGES/dreamlang.mo
    touch share/locale/en_US/LC_MESSAGES/dreamlang.mo
fi

# 创建构建目录
echo "创建构建目录..."
mkdir -p build
cd build

# 运行 CMake
echo "运行 CMake 配置..."
cmake ..

# 编译项目
echo "编译项目..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# 复制国际化文件到构建目录
echo "复制国际化文件..."
cp -r ../share .

echo ""
echo "构建完成！"
echo "可执行文件位置: build/dreamlang"
echo ""
echo "使用示例:"
echo "  ./dreamlang examples/calculator.zv"
echo "  ./dreamlang -l zh_CN examples/calculator.zv"
echo "  ./dreamlang -t examples/calculator.zv"
echo "  ./dreamlang examples/calculator    # 自动添加 .zv 后缀"
echo ""
echo "获取帮助:"
echo "  ./dreamlang --help"
echo "  ./dreamlang -l zh_CN --help"
