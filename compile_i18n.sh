#!/bin/bash

# DreamLang 国际化编译脚本
# 将 .po 文件编译为 .mo 文件

set -e

# 定义语言列表
LANGUAGES=("zh_CN" "en_US")

# 创建locale目录结构
echo "创建本地化目录结构..."
for lang in "${LANGUAGES[@]}"; do
    mkdir -p "share/locale/$lang/LC_MESSAGES"
done

# 编译 .po 文件为 .mo 文件
echo "编译翻译文件..."
for lang in "${LANGUAGES[@]}"; do
    if [ -f "po/$lang.po" ]; then
        echo "编译 $lang.po -> $lang.mo"
        msgfmt "po/$lang.po" -o "share/locale/$lang/LC_MESSAGES/dreamlang.mo"
        if [ $? -eq 0 ]; then
            echo "✓ $lang 编译成功"
        else
            echo "✗ $lang 编译失败"
        fi
    else
        echo "警告: po/$lang.po 文件不存在"
    fi
done

echo "国际化文件编译完成！"

# 显示生成的文件
echo ""
echo "生成的文件:"
find share/locale -name "*.mo" -type f
