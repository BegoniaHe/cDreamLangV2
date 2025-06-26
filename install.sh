#!/bin/bash

# DreamLang V2 安装脚本

set -e

PREFIX="${PREFIX:-/usr/local}"
INSTALL_DIR="$PREFIX/bin"
LOCALE_DIR="$PREFIX/share/locale"

echo "DreamLang V2 安装脚本"
echo "===================="
echo "安装目录: $INSTALL_DIR"
echo "本地化目录: $LOCALE_DIR"
echo ""

# 检查是否已构建
if [ ! -f "build/dreamlang" ]; then
    echo "错误: 找不到可执行文件 build/dreamlang"
    echo "请先运行 ./build.sh 构建项目。"
    exit 1
fi

# 检查权限
if [ ! -w "$PREFIX" ]; then
    echo "错误: 没有写入 $PREFIX 的权限"
    echo "请使用 sudo 运行此脚本或设置不同的 PREFIX"
    echo "例如: PREFIX=\$HOME/.local ./install.sh"
    exit 1
fi

# 创建目录
echo "创建安装目录..."
mkdir -p "$INSTALL_DIR"
mkdir -p "$LOCALE_DIR"

# 安装可执行文件
echo "安装可执行文件..."
cp build/dreamlang "$INSTALL_DIR/"
chmod +x "$INSTALL_DIR/dreamlang"

# 安装本地化文件
echo "安装本地化文件..."
if [ -d "share/locale" ]; then
    cp -r share/locale/* "$LOCALE_DIR/"
else
    echo "警告: 找不到本地化文件目录 share/locale"
fi

echo ""
echo "安装完成！"
echo ""
echo "可执行文件: $INSTALL_DIR/dreamlang"
echo "本地化文件: $LOCALE_DIR/*/LC_MESSAGES/dreamlang.mo"
echo ""
echo "使用方法:"
echo "  dreamlang examples/calculator.dl"
echo "  dreamlang -l zh_CN examples/calculator.dl"
echo ""
echo "如果命令未找到，请确保 $INSTALL_DIR 在您的 PATH 中。"
echo "您可以将以下行添加到您的 shell 配置文件中:"
echo "  export PATH=\"$INSTALL_DIR:\$PATH\""
