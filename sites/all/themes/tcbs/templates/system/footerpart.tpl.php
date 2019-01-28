<footer id = "footer" class = "footer-section section footer-dark-blue">
  <?php if ($page['footer']): ?>
  <div class = "footer-subscribe">
  <div class = "container">
      <div class = "row">
        <div class = "col-md-12">
        <?php print render($page['footer']); ?>
        </div>
      </div>
  </div>
  </div>
  <?php endif; ?>
  <?php if ($page['footer_message'] || $page['footer_menu_1'] || 
    $page['footer_menu_2'] || $page['footer_menu_3'] || 
    $page['footer_menu'] || $page['footer_copyright'] || 
    $page['footer_social']): ?>
  <div class = "footer-message-menus-copyright-social xs-text-center">
    <?php if ($page['footer_message'] || $page['footer_menu_1'] || 
      $page['footer_menu_2'] || $page['footer_menu_3']): ?>
    <div class = "container footer-blocks">
      <div class = "row vertical-menu">

        <div class = "col-sm-8 col-xs-12 footer-menus ">
          <div class = "row">
            <?php if ($page['footer_menu_1']): ?>
            <div class = "col-sm-4 col-xs-12 footer-menu footer-menu-1">
              <?php print render($page['footer_menu_1']); ?>
            </div>
            <?php endif; ?>
            <?php if ($page['footer_menu_2']): ?>
            <div class = "col-sm-4 col-xs-12 footer-menu footer-menu-2">
              <?php print render($page['footer_menu_2']); ?>
            </div>
            <?php endif; ?>
            <?php if ($page['footer_menu_3']): ?>
            <div class = "col-sm-4 col-xs-12 footer-menu footer-menu-3">
              <?php print render($page['footer_menu_3']); ?>
            </div>
            <?php endif; ?>
          </div>
        </div>
        <?php if ($page['footer_message']): ?>
        <div class = "col-sm-4 col-xs-12 footer-message">
          <?php print render($page['footer_message']); ?>
        </div>
        <?php endif; ?>        
      </div>
    </div>
    <?php endif; ?>
    <?php if ($page['footer_menu'] || $page['footer_copyright'] || 
      $page['footer_social']): ?>
    <div class = "footer-copyright-menu-social container horizontal-menu">
      <div class = "row">
        <?php if ($page['footer_menu']): ?>
        <div class = "col-sm-6 col-xs-12 footer-menu text-left ">
          <?php print render($page['footer_menu']); ?>
        </div>
        <?php endif; ?>
        <?php if ($page['footer_copyright']): ?>
        <div class = "col-sm-6 col-xs-12 footer-copyright text-center">
          <?php print render($page['footer_copyright']); ?>
        </div>
        <?php endif; ?>
                
        <div class = "col-sm-6 col-xs-12 footer-social text-right">
          <ul class="list-inline" style="font-size: 17px;">
          <li><a href = "#" target = "_blank"><i class="fab fa-twitter"></i></a></li>  
          <li><a href = "#" target = "_blank"><i class="fab fa-facebook"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-medium"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-reddit-alien"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-youtube"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-linkedin-in"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-instagram"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-ethereum"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-bitcoin"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-github"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-telegram-plane"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-discord"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-google-plus-g"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-behance"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-dribbble"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-pinterest-p"></i></a></li>
          <li><a href = "#" target = "_blank"><i class="fab fa-product-hunt"></i></a></li>
          </ul>

        </div>
        
      </div>
    </div>
    <?php endif; ?>
  </div>
  <?php endif; ?>

  

</footer>
