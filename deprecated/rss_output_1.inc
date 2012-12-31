<?php

//
// RSS1.0アウトプット部分
// テンプレートだけ決めてあげればOK
//
/**
 * @deprecated
*/

require_once("rss_output_base.inc");

class rss_output_1 extends rss_output_base
{
// public 

public function __construct() {
  parent::__construct();
}

/**
 * RSS1.0用のテンプレートを返す
 *
 */
protected function rss_template() {

  // XXX ヒアドキュメント使ってるけど、問題あったら変更して…
  // (これだと'$'のエスケープが必須)

  return <<<END_OF_TEMPLATE
<?xml version="1.0" encoding="utf-8" ?>
$$\$channel_css_put$$$
<?xml-stylesheet type="text/css" href="%%%channel_css_url%%%" ?>
$$\$/channel_css_put$$$
<rdf:RDF 
  xmlns="http://purl.org/rss/1.0/"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" 
  xmlns:content="http://purl.org/rss/1.0/modules/content/"
  xml:lang="%%%channel_language%%%">

 <channel rdf:about="%%%channel_about%%%">
  <title>%%%channel_title%%%</title>
  <link>%%%channel_link%%%</link>
  <description>%%%channel_description%%%</description>
  <items>
   <rdf:Seq>
    $$\$li_loop$$$
    <rdf:li rdf:resource="%%%li_resource%%%"/>
    $$\$/li_loop$$$
   </rdf:Seq>
  </items>
  <image rdf:resource="%%%image_resource%%%"/>
  <textinput rdf:resource="%%%textinput_resource%%%"/>
 </channel>

 <image rdf:about="%%%image_about%%%">
  <title>%%%image_title%%%</title>
  <link>%%%image_link%%%</link>
  <url>%%%image_url%%%</url>
 </image>

 <textinput rdf:about="%%%textinput_about%%%">
  <title>%%%textinput_title%%%</title>
  <description>%%%textinput_description%%%</description>
  <name>%%%textinput_name%%%</name>
  <link>%%%textinput_link%%%</link>
 </textinput>

 $$\$item_loop$$$
 <item rdf:about="%%%item_about%%%">
  <title>%%%item_title%%%</title>
  <link>%%%item_link%%%</link>
  <description>%%%item_description%%%</description>
  <content:encoded><![CDATA[
%%%item_contents%%%
]]></content:encoded>
 </item>
 $$\$/item_loop$$$

</rdf:RDF>
END_OF_TEMPLATE;
}

};  // end of class

