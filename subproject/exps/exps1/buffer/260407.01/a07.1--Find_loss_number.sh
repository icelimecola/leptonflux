#!/bin/bash

#---- 251026
# dir_prefix=27179
# nperfold=1000
# nmin=1
# nmax=16000
#---- 251026
# dir_prefix=2811978
# dir_name=$dir_prefix*
# nperfold=2000
# nmin=16001
# nmax=17754
#---- 251026
# dir_prefix=2893572
# dir_name=$dir_prefix*
# nperfold=1000
# nmin=13001
# nmax=14000
#---- 251026
dir_prefix=2893573
dir_name=$dir_prefix*
nperfold=1000
nmin=14001
nmax=15000
#---- 251105
dir_prefix=2927716
dir_name=$dir_prefix*
nperfold=1000
nmin=12001
nmax=13000
#---- 251105
dir_prefix=2928607
dir_name=$dir_prefix*
nperfold=1000
nmin=11001
nmax=12000
#---- 251107
dir_prefix=2976183
dir_name=$dir_prefix*
nperfold=1000
nmin=13001
nmax=14000
#---- 251107
dir_prefix=01.1
dir_name=$dir_prefix
nperfold=2000
nmin=1
nmax=2000
#---- 260407
dir_prefix=dataiss
dir_name=$dir_prefix
nperfold=1431
nmin=1
nmax=1431


# ---------- 1. 确定总文件夹（取第一个匹配目录） ----------
first_dir=$(ls -d ./$dir_name/ 2>/dev/null | sort | head -1)
[[ -z "$first_dir" ]] && { echo "错误：未找到任何 ./$first_dir*/ 目录！"; exit 1; }
first_name=$(basename "$first_dir")
output_root="a07-${first_name}"

mkdir -p "$output_root"

# ---------- 2. 生成全局 all_numbers（放在总文件夹：a07-all_numbers.txt） ----------
global_all="${output_root}/a07-all_numbers.txt"
{
    echo "=== 生成所有可能的序号 ($nmin-$nmax) ==="
    seq $nmin $nmax | awk '{printf "%05d\n",$1}' > "$global_all"
    echo "all_numbers (全局): $(wc -l < "$global_all")"
    echo
} > "${output_root}/a07-output.txt"

# ---------- 3. 收集所有目录并排序 ----------
mapfile -t dirs < <(ls -d ./$dir_prefix*/ 2>/dev/null | sort)

# 用于累计所有缺失号
all_missing_tmp=$(mktemp)

# ---------- 4. 遍历每个目录 ----------
for i in "${!dirs[@]}"; do
    dir="${dirs[$i]}"
    dir_name=$(basename "$dir")

    # 子目录
    subdir="${output_root}/${dir_name}"
    mkdir -p "$subdir"

    # 编号范围
    start=$((i * nperfold + nmin))
    end=$(( (i + 1) * nperfold + nmin))
    (( end > nmax )) && end=$nmax
    echo "=== $dir_name 生成所有可能的序号 ($start-$end) ==="

    # ---- 本目录的 all_numbers ----
    all_file="${subdir}/a07-all_numbers_${dir_name}.txt"
    seq "$start" "$end" | awk '{printf "%05d\n",$1}' > "$all_file"
    all_cnt=$(wc -l < "$all_file")

    # ---- 提取 existing ----
    existing_file="${subdir}/a07-existing_${dir_name}.txt"
    {
        echo "=== 目录: $dir_name (预期范围: $(printf "%05d" $start) - $(printf "%05d" $end)) ==="
        ls "${dir}"expshist_*_*.root 2>/dev/null | \
            grep -oP '(?<=expshist_)\d{5}(?=_)' | \
            sort -n > "$existing_file"

        exist_cnt=$(wc -l < "$existing_file" 2>/dev/null || echo 0)
        echo "existing_numbers ($dir_name): $exist_cnt / $all_cnt"

        # ---- 本目录 missing ----
        missing_file="${subdir}/a07-missing_${dir_name}.txt"
        comm -23 <(sort "$all_file") <(sort "$existing_file") | sed 's/^0*//' > "$missing_file"
        miss_cnt=$(wc -l < "$missing_file")

        # 累计到全局缺失文件
        cat "$missing_file" >> "$all_missing_tmp"

        echo "missing_numbers ($dir_name): $miss_cnt"
        if (( miss_cnt > 0 )); then
            echo "   缺失示例: $(head -5 "$missing_file" | tr '\n' ' ' || echo "无")...（共 $miss_cnt 个）"
        else
            echo "   无缺失文件"
        fi
        echo
    } >> "${output_root}/a07-output.txt"
done

# ---------- 5. 生成全局缺失文件 ----------
global_missing="${output_root}/a07-missing.txt"
sort -n "$all_missing_tmp" | uniq > "$global_missing"
rm -f "$all_missing_tmp"

# ---------- 6. 全局汇总 ----------
{
    echo "=== 全局汇总 ==="
    total_exist=0 total_miss=0
    for f in "${output_root}"/*/a07-existing_*.txt; do
        [[ -f "$f" ]] || continue
        total_exist=$((total_exist + $(wc -l < "$f")))
    done
    for f in "${output_root}"/*/a07-missing_*.txt; do
        [[ -f "$f" ]] || continue
        total_miss=$((total_miss + $(wc -l < "$f")))
    done

    echo "所有目录总计:"
    echo "  已有文件: $total_exist"
    echo "  缺失文件: $total_miss"
    echo "  完整度: $(awk "BEGIN {printf \"%.2f%%\", ($total_exist/($nmax-$nmin+1))*100}")"
    echo
    echo "全局缺失文件: $global_missing  (共 $total_miss 条)"
    echo "所有结果已整理至: $output_root/"
    echo "日志文件: ${output_root}/a07-output.txt"
} >> "${output_root}/a07-output.txt"

# ---------- 结束提示 ----------
echo "完成！"
echo "   总文件夹: $output_root/"
echo "   全局 all_numbers: $global_all"
echo "   全局 missing   : $global_missing"
echo "   日志文件      : ${output_root}/a07-output.txt"
