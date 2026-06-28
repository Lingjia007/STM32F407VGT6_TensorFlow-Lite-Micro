"""
将 testdata 目录下的 1 秒 WAV 文件转换为 TFLM micro_speech 格式的 .cpp / .h 文件。

输出格式示例（与 yes_1000ms_audio_data.cpp/h 一致）：
  - .h: #include <cstdint>  +  constexpr size + extern const int16_t array[]
  - .cpp: #include <cstdint>  +  #include header  +  alignas(16) const int16_t array[] = {...};

用法:
  python wav_to_cpp.py
"""

import wave
import struct
import os
import glob

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TARGET_SR = 16000      # 目标采样率
TARGET_DURATION = 1.0  # 目标时长（秒）
TARGET_FRAMES = int(TARGET_SR * TARGET_DURATION)  # 16000


def wav_to_cpp(wav_path: str):
    """读取一个 16-bit PCM WAV，输出对应的 .cpp 和 .h 文件。"""
    basename = os.path.splitext(os.path.basename(wav_path))[0]  # e.g. "go"
    var_name = f"g_{basename}_1000ms_audio_data"
    h_filename = basename + "_1000ms_audio_data.h"
    cpp_filename = basename + "_1000ms_audio_data.cpp"

    with wave.open(wav_path, "rb") as wf:
        nch = wf.getnchannels()
        sw = wf.getsampwidth()
        sr = wf.getframerate()
        nf = wf.getnframes()
        raw = wf.readframes(nf)

    # 验证格式
    if sr != TARGET_SR:
        print(f"  [SKIP] {basename}: sample rate {sr} != {TARGET_SR}")
        return
    if sw != 2:
        print(f"  [SKIP] {basename}: sample width {sw} != 2 (16-bit)")
        return
    if nf < TARGET_FRAMES * 0.5:
        print(f"  [SKIP] {basename}: frames {nf} too short (less than 50% of {TARGET_FRAMES})")
        return

    # 解码为 int16 列表
    samples = struct.unpack(f"<{nf * nch}h", raw)

    # 多声道取第一个声道
    if nch > 1:
        samples = samples[::nch]

    # 截断 / 填零到目标帧数
    if len(samples) > TARGET_FRAMES:
        samples = samples[:TARGET_FRAMES]
    elif len(samples) < TARGET_FRAMES:
        samples = list(samples) + [0] * (TARGET_FRAMES - len(samples))

    data_size = len(samples)

    # --- 写 .h ---
    h_path = os.path.join(SCRIPT_DIR, h_filename)
    h_content = (
        "#include <cstdint>\n"
        "\n"
        f"constexpr unsigned int {var_name}_size = {data_size};\n"
        f"extern const int16_t {var_name}[];\n"
    )
    with open(h_path, "w", encoding="utf-8") as f:
        f.write(h_content)
    print(f"  [OK] {h_filename}")

    # --- 写 .cpp ---
    cpp_path = os.path.join(SCRIPT_DIR, cpp_filename)
    data_str = ",".join(str(s) for s in samples)
    cpp_content = (
        "#include <cstdint>\n"
        "\n"
        f'#include "testdata/{h_filename}"\n'
        "\n"
        f"alignas(16) const int16_t {var_name}[] = {{{data_str}}};\n"
    )
    with open(cpp_path, "w", encoding="utf-8") as f:
        f.write(cpp_content)
    print(f"  [OK] {cpp_filename}")


def main():
    wav_files = sorted(glob.glob(os.path.join(SCRIPT_DIR, "*.wav")))
    if not wav_files:
        print("No WAV files found.")
        return

    print(f"Found {len(wav_files)} WAV file(s) in {SCRIPT_DIR}\n")
    for wav_path in wav_files:
        print(f"Processing {os.path.basename(wav_path)} ...")
        wav_to_cpp(wav_path)
    print("\nDone.")


if __name__ == "__main__":
    main()
