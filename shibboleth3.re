= Shibboleth IdP v3

== はじめに

ようやくShibboleth IdP v3が発表されたと思ったら、2016年7月31日をもってShibboleth IdP v2のサポートを一切やめるというアナウンスまで付いてきました。本書は2016年6月25日のイベント向けに書かれていますが、そこから1ヶ月後にはv2は「放置」の状態に突入します。脆弱性が発見されても修正はされません。v2とv3では互換性はありません。

Idp Version 3.0.0のリリースは2014年12月22日です。以後もそれなりに重大なバグが修正されるなどして、本稿執筆時点の最新版である Version 3.2.1 は2015年12月19日にリリースされました。Version 3.2.1 は、筆者の主観では今のところ悪くない印象ですけれども、それにしても2年弱程度でのIdP入れ替えを企業や大学全体に強いるのは「強気だなぁ」と、遠目に見て思いました。

逆に言えば、そのくらいの速度感でソフトウェア入れ替えを出来ないようなら、LinuxにShibboleth IdPを入れて自力でIdPを運用するのはNGな時代なのかもしれません。海外でIdPを持つ大学さんは、そういう速度感でサーバをメンテナンス出来る職員がいることが前提、なのでしょうね。他のOSS IdP実装の速度感は筆者には分からないのですが、状況は似たり寄ったりなのではないかなぁ、という印象です。

SAMLについて言えば、既にクラウドサービスもオンプレミス製品も出ています。今回Shibboleth IdP v2が早々サポート対象外になるという現象は、出来合いの製品やクラウドサービスに頼ることに改めて追い風を与えた印象を持ちました。

そう考えると、わざわざこの本を出していながらも「Shibboleth IdPを自力で運用するっていう路線の未来はどうなんだろう？」なんて思わなくもないんですけど……そんなことは気にせず、この本はSAMLとりわけShibboleth IdPに捧げた本です。

さて本章では、IdP v3に関わる話題で良くあるトピックや、個人的に「マジでー？」と思ったようなことをつらつら書いていきます。


== 「Shibboleth SP 使ってるんだけど、大変なことが起きたのだな！？」

多分、貴公が考えていることとは意味が違います。本章ではIdP Version 3のことを考えていきますが、その前に本節で、SPのことを先に考えておきます。

たまに勘違いされるところらしいので本稿でも書いておくと、Shibboleth SPを利用してSAML連携を行うサービスを作っている会社の場合、今回のShibboleth IdP v2のEOL（End of Life）の話はそこまで心配することではありません。

Shibbolethという名前で、IdPとSP双方に対して完全に別のソフトが開発・公開されています。今回、そのうちのIdPの実装が大きく更新されました。しかしSPにはそのような大規模な更新は起きていません。SPとIdP間の通信の軸になるSAMLプロトコルが変わったわけでもないので「SPの実装を入れ替え、それに依存したソフトも大規模に修正しなければならない」という事態はSP側には起きていません。引き続き、Shibboleth SP 2系をお使いください。

=== 属性送信について挙動が変わった？

というわけで原則としては、Shibboleth IdP変革の騒ぎにSPが巻き込まれることはないはずです。

ただし、SP・IdP間で使用するSAMLプロトコル自体は変わらないものの、それは「プロトコルに曖昧さが全く無く、IdP v2もv3も100%全く同じデータを送ってくる」ということは意味しません。ですのでSP側の人が本当に何もせずに済むかどうか、というのは実は分からないのでした。

例えば、IdPのバージョン更新を機に、SPとIdP間の受け渡し規約が変わるケースがあります。2016年5月27日に行われたNIIオープンフォーラム「学認とShibboleth IdP ver.3」というトラックにおけるNIIの西村健先生の発表で「eduPersonTargetedID」がdeprecated（非推奨）となり、NameIDに取って代わる件が議論されていました。該当する拡張属性を使っている場合、SPでも参照する部分を微調整する必要があるかもしれません。IdPの運用者と、交換する属性情報について変更がないかを確認するくらいは、当然した方が良いでしょう。

また、Shibboleth IdP v3から標準機能として提供されるようになった「ユーザごとの属性送信の許可機能」がSPに副作用を持ち込む恐れはあります。

この機能はIdP v3を新規インストールした場合には自動で有効化されます。で、有効化されている状況だと、各ユーザはIdP内でログイン後に「IdPは俺のメアドをSPに送ろうとしてるけど、私は許さない！」とユーザ自身で決めてしまえたりします。同じIdPだったはずが、突然あるときにユーザのメールアドレスだけ属性情報として届いてこない、なんていうことも起こり得ます。SP側を実装している人は、そういうことが突然発生する可能性を認識しておく必要があります。単にIdP運用者に「その機能、無効化してくれませんか？」と言っておけば良い話ですが。

まとめると、

 * SP自体が大規模アップデートされたわけではない
 * SAMLプロトコルが更新されたわけでもない
 * 念のため、IdPとの運用レベルでの取り決めのチェックはした方が良い

といったところです。SPについては以上です。


== v2からv3へ移行時の各種注意

ここからは本丸であるIdP Version3の話に移ります。特に、IdP v2からv3へ移行するケースを考えます。新規インストールする場合にも役に立つ知見も同時に書いていきますが、主眼はIdPバージョンの移行です。

スムーズに移行できるかどうか、IdPを運用している組織の事情毎に異なるのは当然です。とはいえ、共通して検討する基本中の基本事項みたいのはあります。本節では、移行するにあたって筆者が思いついた・気づいた点などを、調べた内容と併せて述べていきます。


=== メモリ要件に一応注意

IdP v2は比較的省メモリでもなんとかなっていました。例えばOS全体で1GBといった環境でも問題なく動作していました。数万IDを管理するIdPではそんな構成で生き残れるとはもちろん思われませんが、なるべくIdPにコストをかけたくない、というケースもあるはずです。検証環境にいたってはメモリ256MBとかでも動いて欲しいところです。

ただし、IdP v2の時点でホスト環境に余裕のある構成がなされていない場合、IdP v3のシステム要件、特にメモリ使用量の要件がしれっと厳しくなっていることに一応注意する必要があります。筆者が検証・確認したところ、結論としては「OS全体のメモリは2GB以上、Tomcatに割り当てるメモリは1.5GB以上を推奨」ということが分かりました。

低メモリ環境下ですと、v3はIdP本体のエラーログだけで捉えられない不可思議な挙動や、原因不明のタイミングでのエラーが発生したりします。よくよくシステム状態をチェックしたら、スワップアウトが大量発生しているようで、要するにスラッシングが起きてました。辛い……辛い。

でもまぁ、十分スキルのある運用者ならスワップアウト関連のチェックは日々やっているはずですから、ここはハマらないかなぁ……という気もしないではないです。freeコマンドの結果見て一発で気づけよ、と言われて軽くしょんぼりです。

ともあれ覚えておきましょう。ID数や対向するSPが少ない場合でも、IdP v3ではメモリ2GBくらいは用意しておくべきです。@<fn>{bigger_system}

//footnote[bigger_system][巨大な組織の場合にはこんな「低レベルプレイ」をしているはずはないと思いますが、メモリ使用量が大きくなっているという点に一応注意して、LB配下の各ホストのキャパシティ設計を見直す位はしたほうが良いでしょう。]


=== OSはどれを選ぶべきか

当座凌ぐ目的であればCentOS 6（RHEL 6）かな、という印象を受けます。ただ、CentOS 7にもう慣れているのなら、多分何の問題もなく移行出来るはずです@<fn>{skilled_operator}。ここでCentOS 6を選んだ理由は次のとおり。

//footnote[skilled_operator][これにかぎらず、レベルの高い運用者にこの同人誌からお勧め出来ることなんてございませんよ……]

 * 本稿執筆時点では学認技術ガイドの案内が CentOS 6 を使っています。国内資料でまとまっている情報は他にはあまり見当たりません
 * v2時代のガイドもCentOS 6で、色々な知見や補助スクリプトに類似性が見られます
 * CentOS 5 のEOLは2017年5月31日で、1年を切っていますので、今使い続けるメリットよりデメリットのほうが多い印象です
 * CentOS 6 のEOLは2020年ですので、感覚的にもまだ余裕があります
 * CentOS 7 は関連するツールが色々アグレッシブに変わっており、そこで足を取られる可能性が一応あります

CentOS 7について筆者が勝手に思う主要トラップは @<code>{systemctl}, @<code>{jouralctl}, @<code>{firewalld}, Apache 2.4 あたりでしょうか。IdPの更新に際して、常時起動サービスの設定方法やソフトウェアFWの設定などでつまづくのは本筋ではない気がします。Linux環境をいじる場合に新しい環境を体験しておくのは当然としても、IdP v3を入れる段階で敢えて同時にCentOS 7としなければならない理由は見当たりません。IdP v3に慣れてからでも十分ではないでしょうか。

筆者個人としてはDebian/Ubuntu系の方が好みです。Shibboleth本家や学認のガイドはDebian系（Ubuntu等含む）へのインストールについては概ね何も言及してくれないのですが、TomcatとAppArmorあたりの挙動さえ気をつければ、Debian系で動作しないなんてことはないと思われます。本質的には、ただのサーブレットコンテナに突っ込むwarファイル1つです@<fn>{sorry_timeout}。

//footnote[sorry_timeout][各OSへのインストールを試しまくる章を設けようと思ったのですが、時間切れ。次回にご期待ください]


=== JDKはどれを選ぶべきか

開発本家の推奨はOracle JDK 8のようです。筆者個人としてもそちらを推します。ただし議論が別れる部分があるので詳述します。いずれにしても、JDK選択においては認識しておくべき注意点が複数あるように思われます。

Shibboleth IdP v3自体は Java 7 でも Java 8 でも動作しますし、Oracle JDKでもOpenJDKでも動作サポートがあります。

Java 7を使う場合、Oracle JDK 7 は選択してはいけません。Oracleによるサポートが切れています。Java 7に留まりたい場合にはOpenJDK 1.7一択となるはずです。OpenJDK 1.7のEOLは2018年6月で、Oracle JDK 7 よりも長くなっています。

IdP v3でもJava 7を選ぶ理由は、筆者が認識する限り1点のみです。@<code>{attribute-resolver.xml}等にJavaScriptで属性設定を書いている場合です。このとき、Java 7とJava 8でJava内のデフォルトのJavaScript実行エンジンがRhinoからNashornへ変わった影響で、@<code>{attribute-resolver.xml}内の記述内容も変える必要が発生します。この記述変更が出来ない事情がある場合、Java 7を選択することになります。この議論はShibboleth IdP v2の時代に起きた話と何ら変わりませんが、もし今回久しぶりにIdP環境を見直すということであれば、再確認が必要な事項の一つです。

以上の事情が当てはまらない場合、Java 8に対応する2つの選択肢、OpenJDK 1.8かOracle JDK 8のどちらかを選ぶのが良いでしょう。

OpenJDKを使うことのメリットは@<code>{yum}コマンドでアップデートを管理できることかと思います。筆者が把握している限り、yumのレポジトリとして使えるOracle JDKの自動インストーラというのはありませんので、手動インストールの手間がかかるはずです。……もし良いのがあったら是非教えて下さい。

OpenJDKについて言えば、CentOS 6.6 ではOpenJDK 1.8をインストール可能です@<fn>{technology_preview}。ただ、開発元ではOpenJDKを採用してもらうことにはやや消極的らしく「OpenJDKでバグレポート送ってきたら『それ、OracleのVMでも起こる？』って突っ返すかもしれないぞ」とか書いてあります@<fn>{openjdk_warning}。

//footnote[technology_preview][「OpenJDK 1.8はRHELとしてTechnology Previewの段階にあるので、1.8を選ぶのは常道とは言えない」という主旨の説明を聞くことがあったので筆者も鵜呑みにしていたのですが、"Red Hat Enterprise Linux 6.6 Release Notes Edition 6"を読む限り"fully supported"とあります。現状では、OpenJDK 1.8の使用を控えるべき理由はなさそうです。そもそもサポートの有無を意識する意味が無いCentOSならなおさらOpenJDK 1.8を忌避する理由はないという印象を持ちます。RHEL 7でも"fully supported"というステータスに違いはありません。]

//footnote[openjdk_warning]["We strongly recommend the use of Oracle's "standard" JVM on all platforms."と述べた上で"You should expect that reports of unexplained problems may be met with a request to reproduce them on Oracle's JVM."とあります（@<href>{https://wiki.shibboleth.net/confluence/display/IDP30/SystemRequirements}）。警告全文を読むとどういうことがあったかは察しがつきますが、ここでは省略]

というわけでOracle JDK 8が良いのでは、という結論になります。至って普通の結論かと思います。

Oracle JDK 8を使用する場合に、EOLはしばらく気にする必要がありませんが、別の注意点があります。Shibboleth IdP v3のシステム要求では「輸出規制にひっかからない限りは "Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files" を入れておくこと」というものがあります。日本はこの輸出規制にはひっかからないので、是非インストールしておいてください。このファイルのインストール方法等の詳細は省きますが、単に特定のファイルを書き換えてTomcatを再起動するくらいで作業は終わります。JDKのマイナーバージョンが変わる度に作業する必要があるので、その点を忘れないように注意してください。

このポリシーファイル入れ替えをしておかない場合に何が起こるのかというと、要はAES256を始めとした「強力な暗号」が、Oracle JDKのデフォルトでは無効化されていて使えません。これをインストールせずに放置しておいた場合、IdP起動時に警告が出るというだけでなく、今後トラストサークル内で強力な暗号を使った証明書等が出回った時などに「この相手と通信するときだけエラーになる」といった不可思議なエラーの原因になるかもしれません。

ポリシーファイルが入っているかを事前に超簡易的に確認する仕組みとして、著者は @<href>{https://github.com/dmiyakawa/java_aes256_check} を作りました。単にAES256で暗号・復号を試すだけなのですが、標準のOracle JDK 8はそれをしようとするだけで例外を発生させてプログラムが異常終了するので、ポリシーファイルの入れ替えがされていないことが分かります。こういったチェックをしなくても、IdP v3の起動時のログを見れば警告から分かるはずです。

ちなみにOpenJDK 1.8では暗号強度についての問題はありませんので、@<code>{yum}コマンドの件とセットで考えると感覚的な「楽さ」はOpenJDKの方が上です。


=== TomcatとJetty、どちらを選ぶべきか

Tomcat 8がお薦めです。

本章執筆現在、Shibboleth IdPの開発元WikiではJetty 9系を推奨しているのですが、個人的にはJetty + Shibboleth IdP v3という構成をとって期待通りにインストールしきれたことがありません。筆者の無力を嘆くしかないですが、何故かうまくいかないんですよね……。

Jetty 9では（Tomcatでは使える）ajpのサポートがなくなったらしく@<fn>{ajp_dropped_on_jetty9}、リバプロ周りもなんかおかしいとか。ちょっと追えてませんが……とにかく国内で良いニュースを聞きません。何で本家が推奨しているのかむしろ不思議な印象すらあります。

//footnote[ajp_dropped_on_jetty9][@<href>{https://wiki.eclipse.org/Jetty/Howto/Configure_AJP13}に "Support for this feature has been dropped with Jetty 9. If you feel this should be brought back please file a bug. "とあります。]

あまり知的な論理ではないですが、Jetty系で必死に頑張ってだめでTomcatに変えたら一瞬で動作した、なんてことがあって以来、この点では筆者はTomcat一択を推すようになりました。今のところ学認のガイドもTomcatを使用するガイドを公開していますので、まぁ、それで大丈夫でしょう。

Tomcatについて、公式WikiにはTomcat 7以降でIdP v3が動作するとあります。しかしTomcat 7で動作させた場合には一部バグがあるとのことです。現時点では結局、Tomcat 8 が良い選択になりそうです。IdP v2の時代もTomcat系（Tomcat 6）でしたので、設定方法やログの位置などにも十分な類似性があります。v2経験者からするとこの点でも楽です。残念ながら、Tomcat 6ではIdP v3は動作しないようです。v2時代がTomcat 6だったのでそのままIdP v3動作して欲しいと期待しても、世の中はうまくいきません。

筆者はTomcatの方がトラブルが少ないとは感じているのですが、特にサーブレットコンテナレベルで軽量化したい・高度なことをしたい場合や、単にJettyの優秀さを買われている方においては、是非積極的にJetty 9 + IdP v3の組み合わせをアピールしていただければとも思っています。ついでにフロントエンドはnginxでお願いします。

Apache Tomcat系を使う場合、公式にはTomcat 8が強く推奨されているサーブレットコンテナです。ただ、Tomcat 7でも動作はするようで、Tomcat 7を採用していたりするインストール資料もあります。本質的な設定の違いはあまりなさそうな印象です。Shibboleth IdP v3を入れるため「だけ」にTomcatを使っているのであれば大差はありません。ただ既に述べた通り、執筆時点で調査した限りではTomcat 7と組み合わせた場合にバグがあるという話が本家Wikiに書かれています。


=== 設定の移行はスムーズか

IdP v3では、インストーラがIdP v2を検出すると、その設定の一部をIdP v3に移行してくれます。ただし「IdP v2のXML文法を解釈して等価なIdP v3の新しい設定に書き換えてくれる」わけではありません。そこまで親切ではありませんでした。ちぇ。

基本的に、一部の設定XMLをそのままコピーして、IdP v3本体は「XMLの構造がv2っぽかったらv2っぽく解釈する」ような感じで動作します。もう少し具体的には、@<code>{attribute-resolver.xml}, @<code>{attribute-filter.xml}等のXMLについて、IdP v3はv2の記述をそのまま受け入れます。IdP v3はXMLにv2の文法が使われているようならば、なるべくv2流に解釈しようとします。よって、その辺りの設定はそのままでも「ほぼ」動きます。

この点で、IdP v3の技術者は移行難易度が低くなるように努力した節がいくらか見受けられます。IdP v2で運用者自身が「自分が何をやっていたのか」を正確に把握できていれば、移行難易度がなるべく下がるよう、それなりに丁寧に設計されている印象です。

IdP v3インストール後、ノータッチでv3が動作するようになることは通常ありません。例えば、LDAPの設定についてはどうしても「いじる」必要が出てきます。IdP v2では（おそらく）独自にLDAP接続周りの処理を実装していたような感じでしたが、IdP v3ではldaptiveという外部ライブラリを採用しています。LDAPについての基本的な設定について、v3でv2と同等な記述はもちろんできますが、記述先としてldap.propertiesという設定ファイルが追加されたのでそちらに設定を書き直す必要があります。またLDAPの基本設定に加えて、ldaptiveに関する設定を追加で行うことが出来、環境によってはそこでもチューニングをする必要が出てくるかもしれません。LDAPSをいわゆる「オレオレ証明書」経由で行っていた場合、v2ではJavaに対してkeytoolで証明書を追加する処理が必要でしたが、IdP v3では設定ファイルに記載すれば良いようです。雑に言えば、移行できる設定はすんなり移行するのだけど、新しいところがないわけじゃない、ということです。設定をそのままでは移行できないので「互換性がある」とはもちろん言えません。

細かい点については組織ごとの事情で移行難易度は大きく異なってくるのではないかと思います。冗長構成に関してはv3の方が簡単になっている側面もあるようで「何もやらなくても冗長構成の挙動が改善した」という事例もあるようです@<fn>{keio_is_good}。一方、例えばv2向けのプラグインを実装していた場合にはそのためのケア（というか再実装）が必要かもしれません。

//footnote[keio_is_good][@<href>{http://www.nii.ac.jp/csi/openforum2016/track/day3_4.html}における細川達己先生の資料]


=== たくさんの警告メッセージ

前述のとおり、IdP v3はv2の文法を設定ファイルが使っていると判断した場合によしなにしてくれたりしますが、XMLの中身を解釈して新しい文法に書き換えてくれたり……なんてことはしません。言い換えると、動いてるのだけどIdP v3の観点から言えば一種の「ゴミ」が設定に混じった状態で動作します。

IdP v3はなるほど起動し動作します。ただし、運用者を攻め立てるようにログに「WARNING」が出まくります。「これは古い書き方だ直せ！！」というのが、起動時に毎回。

これこそ、開発者側の意図通りの動作のようです。移行ガイド（@<href>{https://wiki.shibboleth.net/confluence/display/IDP30/UpgradingFromV2} ）を見るとそのように書かれています。要は「なるべく長いダウンタイムが起こらないように、素早くIdP v2からIdP v3移行できるように工夫したけれども、古い設定については警告を出しておくので、IdP v3に移行できたら、そのあと時間を取って順次設定を直していってくりゃれ？」という意図のようです。

規模が大きいIdPですと、そういう段階を踏まないと相応の技術職員が在籍している状況でもIdP v2 EOLまでに間に合わないという事情はあり得ます。その場合には「移行してから順次設定を書き換えていく」アプローチを採るのは十分妥当な選択肢と思われます。

一方、移行前後の設定でどこを変えれば良いのかわかっていて、検証する時間も一度に更新し切る余力もあるのならば、IdP v2の設定を元にして、IdP v3に初めから書き直す、というのも妥当な判断じゃないかと思われました。そうしないと、いつまでたっても本物の警告とv2の亡霊の見分けをつけられない気もします。IdPの「移行」と言いつつ、ほぼ新規インストールと同じ作業になりますが、IdP規模や構成によってはそっちのほうが結局は楽かもしれません。


== Version 3.2.1 で「安定」したのか

メーリングリストや他の資料を眺めていると、IdP v3系の少し前のバージョンでは、必要な機能がなかったりインストール時の設定ファイル自体に微細なバグが入っていたりと、色々「アレ」な印象を受けたのですが、筆者が自分で扱っている範囲で言えば、Version 3.2.1はそれなりにトラブルが少ないという意味でそこそこ「安定」している印象を受けます。先行した方々の各種開拓とノウハウ共有のおかげ、という面も大きいです。

ここで「今後、v3で危なっかしい変更が行われる可能性はないか」という疑問が筆者的には自然と浮かびます。安定し続けてくれればもちろん嬉しいのですが、Version 3.2.1までの事情をチラ見する限りですと、開発者のスタイルには少し「拙速」な印象を持たないでもなかったりします。

で、一節によると、開発元は Version 3.3のリリースに向けてアクティブに開発を進めているとのことです。言い換えると、そこでまた「うわー！」という変更が行われる可能性はゼロではない、ということです。

「v2からv3に移行した！」と安心していたのもつかの間、Version 3.3が破壊的変更とセキュリティ修正がセットになってリリースされる……なんていう事態もある「かも」しれません。

と言っても、IdP v2が来月終わるわけですから、v2で待機する、という選択肢はもう現時点ではないと考えるしかありません。ごちゃごちゃ言っても仕方ないかな……


== さいごに

Shibboleth IdP 3の情報源はそこまで潤沢とは言えない部分がありますが、有志の方々の積極的な情報共有によって急速に道路が舗装されている印象です。主に「学認」周囲の情報を元に観測して限りですと、以下の一連の情報源が特に役に立ちました。誠に頭が下がる思いです。

 * @<href>{https://www.gakunin.jp/ml-archives/upki-fed/}
 * @<href>{http://www.nii.ac.jp/csi/openforum2016/track/day3_4.html}
 * @<href>{http://memo.itc.keio.ac.jp/blog/?p=474}
 * @<href>{https://www.gakunin.jp/ml-archives/upki-fed/}
 * @<href>{https://wiki.shibboleth.net/confluence/display/IDP30}
 * @<href>{https://meatwiki.nii.ac.jp/confluence/display/GakuNinShare/Shibboleth+IdP+3}
 * 学認技術ガイド

なお、特別お勧めはしませんが、筆者がメモ代わりに使っているページが @<href>{https://mowa-net.jp/wiki/ShibbolethIdP3} にあります。

