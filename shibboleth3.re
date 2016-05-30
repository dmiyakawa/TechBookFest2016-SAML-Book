= Shibboleth IdP v3

随分前からShibboelth v3のことが語られていました。
昨年ついにv3が発表されたと思ったら、2016年7月31日をもってv2のサポートを一切やめることになりましたと。

これは筆者的にも結構驚きでした。
海外の事情ははっきり分かりませんが、
SAMLを入れている組織というのは概ね数年に一度というスパンで予算がついて、
予算取りの後に設計とか色々入って、ようやくシステム入れ替えしたりします。

旧バージョンと新バージョンが混在して良い期間を長めに取らないと、
そういうことをやる暇なく各IdPの運用者が独自に入れ替えねばなりません。
これは遠目に見て「大変だなぁ」と思いました。

逆に言えば、今後はこのくらいの速度感でソフトウェア入れ替えを出来ないようなら
CentOSなりRHELなりオンプレのサーバに入れて自力でIdPを運用するのはNGな時代かもしれません。

現実に、SAMLという文脈で言えばクラウドサービスもオンプレミスソリューションも出揃って久しいですから、
そういう出来合いのものを頼るのが一般化するきっかけに（逆に）なったのかなぁと思います。
また、海外でIdPを持つ大学さんはそういうことをやれる職員がいることが前提なんでしょう。

そう考えると、わざわざ同人誌出していながらも「Shibbolethの日本での未来はどうなんだろう？」なんて思わなくもないんですけど、この本はSAMLとりわけShibbolethの本なのです。

== Shibboleth SP使ってるんだけど大変なことが起きたのだな！？

違います。

知り合いにSP側でサービスを提供される方がいたりして、たまに勘違いされるところらしいので、ここでも記載しておきます。SP（Service Provider）側でShibboleth SPを採用している会社さんは今回のShibboleth IdP v2のEOLについて、ほとんど関係しません。

SAMLについて復習しておくと

 * ユーザを認証してその認証情報を提供するIdP（Identity Provider）
 * 提供されたユーザ情報を使って実際のオンラインサービスを提供するSP（Service Provider）

のふた手に別れます。

Shibbolethについて言えば、"Shibboleth"という名前で、IdPとSP双方に対して完全に別のソフトが提供されていて、今回IdP側が大きく変わっただけです。通信の軸になるSAMLプロトコルが変わったわけではないので、SP側で大きく変わったりしたお話はありません。

=== 属性送信について少しだけ挙動が変わったかも？

原則としては「SPがこのIdP変革の騒ぎに巻き込まれることはない」というのが正しいんですが、
Shibboleth IdPの機能として標準で提供されるようになったユーザごとの属性送信の許可機能が
SPに副作用を持ち込む恐れはあります。

要は「IdPは俺のメアドをSPに送ろうとしてるけど、許さない！」とユーザが決めてしまえたりします。
すると、ID上は同じIdPだったのに、突然あるときにメアドが届いてこない、なんていうことも。
SP側を実装している人はそういうことも起こりえることは常に意識しておく必要があります。

ただまぁ、これって「uApprove.JP」という学認様が提供している機能を使えばv2時代からあった
挙動なので、今更SP側で驚く人がいたらそれはそれでちょっとまずいと思います。



== Shibboleth IdP v2からの移行難易度

v2で何をやっていたかが正確に把握できていれば、移行難易度がなるべく下がるようにかなり丁寧に設計されているようです。

流石リードタイムを短くしただけのことはあるのか、attribute-resolver.xml, attribute-filter.xml等はほぼそのまま受け入れますし、

冗長構成に関して言うならば秘密鍵をサーバ間で共有することで、クライアントサイドにセッション情報を暗号化した状態で持つという設計がデフォルトになったおかげで「逆に何もやらなくて良くなった」という大学さんの例もあるようです（慶應）。

とはいえ全てがスムーズに行くかは単純に大学の事情毎に異なるでしょう。
例えば独自プラグインを実装していた場合、独自プラグイン部分はv3の仕組みの仕組みにあわせて再実装か諦めるかしないとダメになるでしょう。

=== OSは何にしよう？

当座、凌ぎきるのであればCentOS 6かな、という印象。RHELでももちろん良いですが。

理由はいくつかありますが

 * 本稿執筆時点では学認技術ガイドの案内が CentOS 6 を使っている
 * サポート終了がまだだいぶ先（CentOS 5はその点まずい）
 * CentOS 7は色々アグレッシブに変わってしまいそっちで足を取られる可能性大

筆者が思う主要トラップは systemctl, jouralctl, firewalld, network, apache (2.4) あたりでしょうか。
当然メジャーバージョンが上がると設定とか全部書き換わるのですが、特にIdPについて言えば、常時起動するサービスの設定だとかソフトウェアFWの設定だとかで躓くのは本筋じゃない気がします。それは別の機会にやったら？

Linux環境をいじる場合には新しい環境を前もって体験しておくのは当然として、IdP v3を入れるんだよー、というときに敢えてCentOS 7でないといけない理由はありません

ちなみにShibboleth本家はあまりDebian系（Ubuntu等含む）について言及してくれませんが、
TomcatとAppArmorあたりの挙動さえ気をつければDebian系に入らないなんてことはありません。
つーか本質的には、ただのサーブレットコンテナに突っ込むwarファイル1つなんで。

=== JDKは何にしよう？

まず、Shibboleth IdP v3自体はJava7でもJava8でも動作します。Oracle JDKでもOpenJDKでも動作します。ただし注意点を認識しておく点が複数あります。

attribute-resolver.xml 等にJavaScriptを書いている場合、Java7とJava8でJava内のデフォルトのJavaScript実行エンジンが変わった（Rhino -> Nashorn）影響で、書き方も変える必要がでてしまいます。

Java7のままでいたい場合、Oracle JDK 7 は「サポートが切れているので使ってはダメです」。なのでOpenJDK 7一択になります。EOLは2018年6月。CentOS 6の方が息が長い（2020年）ので、CentOS 6 + OpenJDK 7で耐え忍ぶ期間はそんなに長く出来ません。

CentOS 6.6 あたりではOpenJDK 1.8が入ってますので使いたいところですが、1.7と異なり「テクノロジープレビュー」の段階である、なんていう説明があったりします。特にRHEL 6であれば1.8でおかしな動作が起きた際には「サポート外」になりますので大きな違いになるはず。CentOS 6だと、まぁ手元の試験環境でおかしなことが起きたことはございません、くらいなもんです。


(Oracle) Java8に乗り換えた場合、EOLは当座気にしなくて良いと思いますが、一点注意があります。
輸出規制にひっかからない限りは
"Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files"を入れておくべき、
というものがあります。日本はひっかからないので是非入れておいてください。

詳細は省きますが、要はAES256みたいな暗号はOracle JDKのデフォルトでは無効化されていて使えないんですね。
これを見落とすと、Shibboleth IdP v3起動時に色々と警告が出ます。

なお、これが入っているかの超簡易のチェッカとして著者は @<href>{https://github.com/dmiyakawa/java_aes256_check} をリリースしました。

=== Tomcat？ Jetty？

Tomcat系がおすすめです。

実は執筆現在本家はJettyをお勧めしているのですが、個人的にはJetty + Shibboleth IdP v3という構成で良いニュースを聞いた試しがありません。ajpは捨てられたっぽく、リバプロ周りもなんかおかしいとか。ちょっと追えてませんが、良いニュースは聞いてません。

理性的な話ではないですが、Jettyで必死に頑張ってだめで、Tomcatに変えたら一瞬で動作した、なんてことがあって以来、この点ではTomcat一択を推すようになりました。

今のところ学認のガイドもTomcat側のようですので、当座はそれで大丈夫でしょう。

特にサーブレットコンテナレベルで高度なことをしたい場合や、Jettyのサーブレットコンテナ自体としての優秀さを買われている方においては、是非積極的にこの辺りでJetty + Shibboleth IdP v3の組み合わせをアピールしていただければと思ってます。

公式にはApache Tomcat 8が公式にサポートされているサーブレットコンテナです。
ただ、Tomcat 7でも動作はするようで、説明によってはTomcat 7を採用していたりする例もあります。
本質的な設定の違いはあまりなさそうな印象。
Shibboleth IdP v3を入れるため「だけ」にTomcatを使っているのであれば、
Tomcat 8を使うか、現時点での学認技術ガイドのようにTomcat 7を使うのも
長いものに巻かれている感じがするので良いのかもしれません。




=== 移行は簡単、ただしたくさんのwarning

v3はv2の文法を使っていると判断したファイルをそのようにあつかったり、v3で仕組みが変わった設定を無視することで移行を実現します。XMLの中身を解釈して新しい文法に書き換えてくれたり……なんてことはしません。

言い換えると、動いてるのだけど「ゴミ」が大量に設定に混じった状態でv3が起動します。

v3は動作します。ただし、運用者を攻め立てるようにログに「WARNING」が出まくります。
「これは古い書き方だ直せ！！」というのがたんまりと。

これは、開発者側の意図通りの動作のようです。https://wiki.shibboleth.net/confluence/display/IDP30/UpgradingFromV2 を見ると"Next Steps"に書かれています。

要は、「素早く移行できるように頑張ったけど、古い設定については警告を出しておくので、v3に素早く移行したら、そのあと時間をとって順次設定を直していってくりゃれ？」という意図です。
決して設定ファイル間に「互換」レイヤがあるような感じではない。

規模が大きい組織ですとそういう段階を踏まないと、v2 EOLのライフスパン上間に合わないというのもありますので、この「移行してから順次設定を書き換えていく」アプローチはあり得ますが、移行前後の設定でどこを変えれば良いのかわかっているのであれば、一度に全部v2の設定を消す、というのも正しい判断なんじゃないでしょうか。そうしないといつまでたっても本物の警告とv2の亡霊の見分けをつけられない感じもします。




=== メモリ要件に一応注意だ

v2は比較的省メモリでもなんとかなっていました。
OS全体で512MBしかなく、スワップアウトも辞さないような猛烈な環境でもそれなりには動きます。

v3ではこのメモリまわりの要件がしれっとかなり大きくなっているのは、あまりにどうでもよいので世間で共有されていない事実のようです。

色々学認関係者にお問い合わせした結果、OS全体のメモリは2GB以上、
Tomcatに割り当てるメモリは1.5GB以上が推奨だよ、ということがわかりました。
技術ガイドにも現在はその旨記載されているはずです。


詳細は省きますが、低メモリ環境下だとv3はIdP本体のエラーメッセージで捉えられない不可思議な挙動や、原因不明のタイミングでのエラーが発生したり辛いです。
著者がハマったことの一つはLDAPの接続プール。
v3ではldaptiveという仕組みをつかっていて、デフォルト設定では接続をプールしとくのですが、
低メモリ下だとなんだかこのプール自体がメモリ圧迫とセットで酷い挙動を示すようなのでした。

これにかぎらず、v3は基本的に「メモリをよく食べる別のJavaソフトウェア」と考えたほうが
辻褄の合う局面がいくつか見受けられますので、いまさら検証するという場合も注意です。

うーん、でもま、普通の運用者ならやってますから、ここはハマらないか……。


== Shibboleth v3.2.1 で「安定」したのだろうか？

v3.3のリリースに向けてアクティブに開発が進んでいるとのことです。

ですので「v2からv3に移行した！」と安心していたのもつかの間、何らかのbreaking change付きでv3.3がセキュリティフィックスと一緒にリリースされる……なんていう事態もあるかもしれません。

とはいえ、今回大幅に変わったbeanやらxxx.propertiesやらが「全部廃止」みたいな状態になったりはしないでしょう。


== オススメの情報源

 * http://www.nii.ac.jp/csi/openforum2016/track/day3_4.html
 * http://memo.itc.keio.ac.jp/blog/?p=474
 * 学認技術ガイド

オススメではありませんが筆者がメモ代わりに使っているページは https://mowa-net.jp/wiki/ShibbolethIdP3 です。

