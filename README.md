# Big Barkin' Dog・デカ犬吠える・大狗叫
 Make BMS louder like a BIG BARKIN' DOG!
 「デカ犬吠える」ように、BMSキー音を大きくしよう！
 让您的BMS像【大狗叫】一样响亮！

# Why did I make this・なぜこれを作るんですか・为什么我要开发这个
In the BOF:NT of 2023, the bms of my work '〇、' was commented that the keysounds had a super low volume. But when I post a fixed version with everything +6db, the audio was horribly clipped. So I decide to writing something with spectral matching technique to solve this forever! I believe this will greatly change the process of BMS producing.
2023のBOF:NTでは、僕の参加作品「〇、」は「キー音の音量は小さすぎです」とコメントされました。しかし、すべでの音を6dbあげると、結果は酷くクリップされてしまいました。そのため、周波数領域マッチングというテックニックで本ツールを開発して、こういう問題を徹底的に解決できると思います！本ツールを利用して、BMS制作流れも徹底的変われる、と信じています。
2023年的BOF：NT比赛中，我的作品【〇、】被一部分玩家评论音量太小，但是当我制作了一版全部提升6db的修正版时、发现最终结果又惨遭削波失真。所以我写了一个基于频域匹配的音量放大工具，这样一来这个问题或许可以彻底解决了！与此同时我也相信，借助本工具也可以彻底改变未来的BMS的制作流程。

# Usage・使い方・用法
1. Load the pre & post mastering wav file. 
 マスタリング前後のwavデータをロードします。必ず二つwavファイルのサンプリングレートと標本数を一致してください。
 加载母带前后的wav文件。请保证所有的音频数据采样率和长度一致。
2. Click 'Analyze'. Keep the 'Soften' box checked and the kernel size & the sigma value to achieve a better result. Generally, the results from the default values are already good.
    「Analyze」をクリックしてロードした音声データ分析します。最優結果のため、Softenをオンにして、カーネルサイズやシグマ値を調整してください。基本的に、デフォルト数値の結果もうかなり良いと思います。
    为了达到最佳效果，请保持Soften开启、适当调整Kernel大小和sigma值。基本上默认值下分析的结果已经是比较好的了。
3. Then select and process all your audio files. There are two modes - track mode and one-shot mode. Prefix is optional. If 'Prefix' box is checked, the prefix of track mode would be 'Dagoujiao_', and the prefix of one-shot mode would be 'Daishuji_'.
    その後、トラックを選択して処理します。「トラック」と「ワンショット」という二つモードがあります。プレフィックスをつけるかとうかのは選べます。つける場合は、「トラック」モードでのプレフィックスは「Dagoujiao_」になって、「ワンショット」モードでは「Daishuji_」になります。
    然后选择并分析您的音频文件。一共有分轨模式和单音模式两种处理方式。前缀可选，勾选使用前缀的情况下，分轨模式的前缀是“Dagoujiao_”，而单音模式的前缀是“Daishuji_”。
4. ogg is supported. The output would be 256kbps by now.
    本ツールではoggが支援されています。現在、出力データの品質は256kbpsになります。
    本工具支持ogg。目前输出的码率是256kbps。
5. I've only build windows version.　You can compile mac version yourself if needed. Feel free to report any bugs by opening an issue!
    Windowsバージョンのみビルドしました。必要な場合はご自身でMacバージョンをビルドしてください。バグ報告はイシューでお気軽にどうぞ！
    目前只编译了Windows版本，Mac版本可以根据需要自行编译。欢迎通过Issue报告Bug！
# License
This tool is licensed under [AGPLv3](https://www.gnu.org/licenses/agpl-3.0.en.html).
## JUCE
https://github.com/juce-framework/JUCE
## KissFFT
https://github.com/mborgerding/kissfft