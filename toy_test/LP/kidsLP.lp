\ENCODING=ISO-8859-1
\Problem name: broken

Minimize
 obj: 500 nbBus40 + 400 nbBus30
Subject To
 ctAllKidsNeedToGo: 40 nbBus40 + 30 nbBus30 - slack1 = 300
Bounds
      nbBus40 >= 0
      nbBus30 >= 0
      slack1 >= 0
End