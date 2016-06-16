= まえがき

この度は『もわねっとのSAML同人誌』を手にとっていただき、本当にありがとうございます！！

本書では、SAML（Security Assertion Markup Language）について、2016年6月中旬頃の情報をもとに書いていきます。

1章では「SAMLって何だっけ？」と称してSAML入門的なことを書きます。SAML自体をご存じない方も、もしご興味があれば是非同章を御覧いただければと思います。

2章では、メジャーバージョンが上がったShibboleth IdP v3について書きます。自分で検証サーバ等をアップデートした際に引っかかったポイントや、各所で拾ったベスト・プラクティス的な情報を書いていきつつ、気になった点を考えていきます。

書名に「SAML」と入れながらも、全体的に「学認」@<fn>{about_gakunin}に偏った話になってますが、ご容赦くださいませ。

//footnote[about_gakunin][「学術認証フェデレーション」略して「学認」。主に大学と大学にオンラインサービスを提供する際にSAML連携を推進するNIIの事業です。SAML関連の技術情報が豊富で助かっています。@<href>{https://www.gakunin.jp/}]

== 文責・お問い合わせについて

文章内容の全責任は筆者（@amedama@<fn>{amedama}）にあります。

//footnote[amedama][@<href>{https://twitter.com/amedama}]

本書のソースコードは@<href>{https://github.com/dmiyakawa/TechBookFest2016-SAML-Book}にて公開予定です。もし内容に気になる点がありましたら気軽にIssueやPull Requestをお願いします。
