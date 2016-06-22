= まえがき

この度は『もわねっとのSAML同人誌』を手にとっていただき、本当にありがとうございます！！

本書はSAML（Security Assertion Markup Language）について書かれた同人誌です。2016年6月中旬時点の情報を元に書いています。二章構成です。

 * 1章では「SAMLって何だっけ？」と称してSAML入門的なことを書きます。SAML自体をご存じない方も、もしご興味があれば是非同章を御覧いただければと思います。
 * 2章では、メジャーバージョンが上がったShibboleth IdP v3について書きます。筆者が検証サーバ等を設置・アップデートした際に引っかかったポイントや、各所で得たベスト・プラクティス的な情報を書いていきながら、筆者として気になった点について考えていきます。

書名に「SAML」と入れながらも、全体的にはShibboleth IdPと「学認」@<fn>{about_gakunin}の事情に偏った内容になってますが、ご容赦くださいませ。

//footnote[about_gakunin][「学術認証フェデレーション」略して「学認」。主に、大学と大学に対してオンラインサービスを提供する企業等のサービス・プロバイダ間でのSAML連携を推進するNIIの事業です。特にShibboleth関連の技術情報が豊富です。@<href>{https://www.gakunin.jp/}]

== 文責・お問い合わせについて

文章内容の全責任は筆者（@amedama@<fn>{amedama}）にあります。

//footnote[amedama][@<href>{https://twitter.com/amedama}]

本書のソースコードは以下のURLにて公開予定です。

 * @<href>{https://github.com/dmiyakawa/TechBookFest2016-SAML-Book}

もし内容に気になる点や誤りがありましたら、気軽にIssue作成やPull Requestをお願いします。
