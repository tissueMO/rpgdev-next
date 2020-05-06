// JavaScript Document

$(function(){
  menuTreeFunc();
  menuNodeFunc();
});

// クリックで展開・折りたたむ
function menuTreeFunc() {
  //var clickSectionNum = 0;
  $("#menuContent .title .arrow").on("click",function(){
    // ＋－アイコン
    $(this).parent().toggleClass("active");

    // サブノード
    $(this).parent().next().toggleClass("active");
  });
}

// 該当ノードを展開
function menuNodeFunc() {
  // strIDが付いたノードであればaddClassする
  var currentSection = $("body").attr("class");
  $("#menuContent ." + currentSection).addClass("active");
  $("#menuContent ." + currentSection).next().addClass("active");
  $("#menuContent ." + currentSection).parent().parent().addClass("active");
  $("#menuContent ." + currentSection).parent().parent().prev().addClass("active");
}
