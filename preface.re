= まえがき

この度は『もわねっとのSAML同人誌』を手にとっていただき、本当にありがとうございます！！

本書では、SAML（Security Assertion Markup Language）について、2016年6月中旬頃の情報をもとに色々書いていきます。「SAMLって何？」という方向けに第一章に「SAML入門」を設けましたので、もしご興味があればそれだけでも是非御覧いただければと思います。

2章以降の主眼は、SAMLのIdPの一実装であるShibboleth IdP、特にVersion3となります@<fn>{maybe_nextime}。この度、メジャーバージョンが2から3に上がって、一部の界隈で盛り上がっていたようです。今回、自分で検証サーバ等をアップデートした際に引っかかったポイントや、各所で拾ったベスト・プラクティスのような情報をメインに書いていきます。また、勝手ですが筆者の都合で、内容は結構「学認」に偏った話が多いです。

//footnote[maybe_nextime][次回があるかのような煽り]

== 参考にした情報ソース等

本書ではあくまで筆者の理解・見解を書いていきますが、「学認」のWebサイトやその関係イベントでの発表、またそこでの質疑応答を通じて得た情報等も記述しています。以下に、特に参考にした代表的な情報源を示します。

 * @<href>{https://meatwiki.nii.ac.jp/confluence/display/GakuNinShare/Shibboleth+IdP+3}
 * @<href>{http://www.nii.ac.jp/csi/openforum2016/track/day3_4.html}
 * @<href>{http://memo.itc.keio.ac.jp/blog/?p=474}
 * @<href>{https://www.gakunin.jp/ml-archives/upki-fed/}

元情報の方が詳しかったり、本書発行後に事情が変わった場合に本Webサイトの方が情報が豊富になったり、ということは起こると思いますので、気になったらブラウザを開きながら御覧ください。


== お問い合わせ先等

文章内容の全責任は筆者（@amedama @<href>{https://twitter.com/amedama}）にあります。

本書のソースコードは@<href>{https://github.com/dmiyakawa/TechBookFest2016-SAML-Book}にて公開予定です。
もし内容に気になる点がありましたら気軽にIssueやPull Requestをお願いします。@<fn>{please_buy_this}


//footnote[please_buy_this][つ、つまり「電子データでいいよ」という方は紙の書籍は買わなくても良いわけですけれども……、是非この機会ですので、興味を持たれましたら購入をご検討いただきたく……（懇願）]

