<?php

//
// RSS2.0アウトプット部分
// テンプレートだけ決めてあげればOK
//
/**
 *
 * @deprecated
*/
require_once("rss_output_base.inc");

class rss_output_2 extends rss_output_base
{
// public 

public function __construct() {
  parent::__construct();
}

/**
 * RSS2.0用のテンプレートを返す
 *
 */
protected function rss_template() {

  // XXX ヒアドキュメント使ってるけど、問題あったら変更して…
  // (これだと'$'のエスケープが必須)

  return <<<END_OF_TEMPLATE
<?xml version="1.0" encoding="UTF-8" ?>
$$\$channel_css_put$$$
<?xml-stylesheet type="text/css" href="%%%channel_css_url%%%" ?>
$$\$/channel_css_put$$$
<rss version="2.0"
  xmlns:content="http://purl.org/rss/1.0/modules/content/" >
 <channel>
  <title>%%%channel_title%%%</title>
  <link>%%%channel_link%%%</link>
  <description>%%%channel_description%%%</description>
  <language>%%%channel_language%%%</language>
  <copyright>%%%channel_copyright%%%</copyright>

  <image>
   <url>%%%image_url%%%</url>
   <title>%%%image_title%%%</title>
   <link>%%%image_link%%%</link>
  </image>

  <textInput>
   <title>%%%textinput_title%%%</title>
   <description>%%%textinput_description%%%</description>
   <name>%%%textinput_name%%%</name>
   <link>%%%textinput_link%%%</link>
  </textInput>

$$\$item_loop$$$
  <item>
   <title>%%%item_title%%%</title>
   <link>%%%item_link%%%</link>
   <description>%%%item_description%%%</description>
   <content:encoded><![CDATA[
%%%item_contents%%%
]]></content:encoded>
   <pubDate>%%%item_pubDate%%%</pubDate>
   <guid>%%%item_guid%%%</guid>
   <enclosure url="%%%enclosure_url%%%" length="%%%enclosure_length%%%" type="%%%enclosure_type%%%" />
  </item>
$$\$/item_loop$$$

 </channel>
</rss>
END_OF_TEMPLATE;
}
                                   
};  // end of class

