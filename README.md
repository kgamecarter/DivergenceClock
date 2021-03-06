# DivergenceClock
## 題目名稱
ダイバージェンスメーター

世界線變動率偵測儀

## 功能
主要功能：用來顯示所在平行世界的變動率。

附加功能：時間、計時器、溫度計、音樂盒、跑馬燈。

## 題目構思
- 如何構成完整的電路？
- 如何焊接電路使其相通並互不干擾？
- 如何把電壓升壓到170V？
- 如何把功能合而為一而不相衝？
- 燈管取得難易度？


## 數字燈管解析
數字管是一種可以顯示數字和其他信息的電子設備。玻璃管中包括一個金屬絲網製成的陽極和多個陰極。大部分數字管陰極的形狀為數字。管中充以低壓氣體，通常大部分為氖加上一些汞和／或氬。給某一個陰極充電，數字管就會發出顏色光，視乎管內的氣體而定，一般都是橙色或綠色。 

儘管在外觀上和真空管相似，其原理並非為加熱陰極放射電子。因而它被稱為冷陰極管或霓虹燈的一個變種。在室溫下，即使處於極端的室內工作條件下，這種管子的溫度很少超過40℃。

數字管的最常見形式有10個陰極，形狀為數字0到9，某些數字管還有一個或兩個小數點。然而也有其他類型的數字管顯示字母、標記和符號。如一種「小精靈管」，其陰極為一個模板製成的面具，上面有數字形狀的孔。一些俄羅斯的數字管，如IN-14，使用倒立的數字2代表5，大概是為了節約生產成本，而沒有明顯的技術或美學方面的原因。

將170伏的直流電壓加在陰極和陽極之間，每一個陰極可以發出氖的的紅橙色光。由於混合氣體的不同，不同類型的數字管之間的顏色有所區別。使用時間較長的數字管在製造中加入了汞，減少了濺射，結果發出的光的顏色為藍色或紫色調。在某些情況下，這些顏色被玻璃上的紅色或橙色過濾塗層過濾。

## 成果預想
- 能夠顯示出世界線（每兩秒更改一次）
- 顯示時間（年、月、日、時、分和秒）
- 碼錶計時（切換途中不中斷）
- 能夠測量溫度並顯示出來
- 播出音樂
- 藍芽通訊設定

## 藍芽通訊封包規格
指令1byte|資料長度1byte|資料0~255byte
-|-|-

- 切換到時間模式
20 00
- 切換到日期模式
21 00
- 設定亮度
13 01 FF
- 設定鬧鐘
05 03 01 30 06

## 照片
![](https://i.imgur.com/EKghmoQ.png)
![](https://i.imgur.com/0zbtbKi.png)
![](https://i.imgur.com/NxFucGy.png)

## DEMO
[![](http://img.youtube.com/vi/TVbxn__EGl4/0.jpg)](http://www.youtube.com/watch?v=TVbxn__EGl4 "")
[![](http://img.youtube.com/vi/hchnT9OHsEU/0.jpg)](http://www.youtube.com/watch?v=hchnT9OHsEU "")
[![](http://img.youtube.com/vi/RF2nmgLNfOo/0.jpg)](http://www.youtube.com/watch?v=RF2nmgLNfOo "")
