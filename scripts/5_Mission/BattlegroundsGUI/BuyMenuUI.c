class BuyMenuUI extends UIScriptedMenu
{
    const int ID_Category = 901;
    const int ID_AddRelated = 2114;
    const int ID_AddCart = 12345678;
    const int ID_RemoveCart = 87654321;
    const int ID_Purchase = 4112;
    const int ID_SubMenu = 21144112;
    const int ID_CloseMenu = 41122114;

    private static ref BuyMenuUI m_Instance;
    private Widget m_BuyMenu;
    private TextWidget m_CurrencyAmount;
    private int m_CurrencyAmountInt;
    private ButtonWidget m_CategoryButton;
    private GridSpacerWidget m_GridSpacerWidgetCat;
    private ButtonWidget m_PurchaseButton;
    protected ScrollWidget m_ScrollWidget;
    protected GridSpacerWidget m_GridSpacerWidget;
    protected ScrollWidget m_ScrollWidgetCart;
    protected GridSpacerWidget m_GridSpacerWidgetCart;
    protected TextWidget m_CartTotal;
    private int m_CartTotalInt;
    const int CART_ITEM_LIMIT = 20;
    private ButtonWidget m_SubMenuButton;
    ref TransferMenuUI m_TransferMenu;
    private ButtonWidget m_CloseMenu;

    ref array<string> uniqueCategories;
    ref map<string, string> categoryImages = new map<string, string>;
    private ref map<Widget, ref PreviewData> m_ItemPreviews = new map<Widget, ref PreviewData>(); 
    private ref map<Widget, int> m_WidgetItemList;
    private ref map<Widget, int> m_WidgetCartList;
    private ref map<int, int> m_ItemQuantities;
    private bool m_ItemsAdded;
    bool m_IsInSafezone = false;

    override Widget Init()
    {
        m_BuyMenu = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BuyMenuUI.layout");
        m_CurrencyAmount = TextWidget.Cast(m_BuyMenu.FindAnyWidget("CurrencyAmount"));
        m_PurchaseButton = ButtonWidget.Cast(m_BuyMenu.FindAnyWidget("PurchaseButton"));
        m_ScrollWidget = ScrollWidget.Cast(m_BuyMenu.FindAnyWidget("ScrollWidget1"));
        m_GridSpacerWidget = GridSpacerWidget.Cast(m_BuyMenu.FindAnyWidget("GridSpacerWidget1"));
        m_ScrollWidgetCart = ScrollWidget.Cast(m_BuyMenu.FindAnyWidget("ScrollWidget2"));
        m_GridSpacerWidgetCart = GridSpacerWidget.Cast(m_BuyMenu.FindAnyWidget("GridSpacerWidget2"));
        m_CartTotal = TextWidget.Cast(m_BuyMenu.FindAnyWidget("CartTotal"));
        m_SubMenuButton = ButtonWidget.Cast(m_BuyMenu.FindAnyWidget("SubMenuButton"));
        m_CloseMenu = ButtonWidget.Cast(m_BuyMenu.FindAnyWidget("CloseMenuButton"));
        m_GridSpacerWidgetCat = GridSpacerWidget.Cast(m_BuyMenu.FindAnyWidget("GridSpacerWidget3"));

        categoryImages.Insert("Weapons", "set:dayz_inventory image:shoulderleft");
        categoryImages.Insert("Attachments", "set:dayz_inventory image:weaponoptics");
        categoryImages.Insert("Magazines", "set:dayz_inventory image:magazine2");
        categoryImages.Insert("Ammunition", "set:dayz_gui image:icon_bullet");
        categoryImages.Insert("Gear", "set:dayz_inventory image:back");
        categoryImages.Insert("Medical", "set:dayz_gui image:iconHealth0");
        categoryImages.Insert("Other", "set:dayz_inventory image:cat_common_cargo");

        m_ScrollWidget.VScrollToPos01(0);
        m_ScrollWidgetCart.VScrollToPos01(0);

        uniqueCategories = new array<string>;
        m_WidgetItemList = new map<Widget, int>;
        m_WidgetCartList = new map<Widget, int>;
        m_ItemQuantities = new map<int, int>;

        PopulateCategories();
        SetCategory(0);
        m_ItemsAdded = true;
        MenuHide();

        return m_BuyMenu;
    }

    void PopulateCategories()
    {
        uniqueCategories.Clear();
        Widget child = m_GridSpacerWidgetCat.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }

        foreach (ItemConfig itemConfig : itemConfigs)
        {
            if (uniqueCategories.Find(itemConfig.category) == -1)
            {
                uniqueCategories.Insert(itemConfig.category);
                ButtonWidget categoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton.layout", m_GridSpacerWidgetCat));
                ImageWidget categoryImage = ImageWidget.Cast(categoryButton.FindAnyWidget("CategoryImage"));
                if (categoryImages.Contains(itemConfig.category))
                { categoryImage.LoadImageFile(0, categoryImages.Get(itemConfig.category)); }
                categoryButton.SetUserID(uniqueCategories.Count() - 1);
            }
        }
    }

    void SetCategory(int categoryIndex)
    {
        Widget child = m_GridSpacerWidgetCat.GetChildren();
        while (child)
        {
            ButtonWidget categoryButton = ButtonWidget.Cast(child);
            categoryButton.SetState(false);
            child = child.GetSibling();
        }

        if (categoryIndex >= 0 && categoryIndex < uniqueCategories.Count())
        {
            ButtonWidget selectedButton = ButtonWidget.Cast(m_GridSpacerWidgetCat.GetChildren());
            for (int i = 0; i < categoryIndex; i++)
            { selectedButton = ButtonWidget.Cast(selectedButton.GetSibling()); }
            if (selectedButton)
            { selectedButton.SetState(true); }

            AddItemToMenu(categoryIndex);
        }
    }

    void AddItemToMenu(int selectedCategoryIndex)
    {
        ClearItemsFromMenu();
        m_ScrollWidget.VScrollToPos01(0);
        string selectedCategory = uniqueCategories[selectedCategoryIndex];
        
        for (int i = 0; i < itemConfigs.Count(); i++)
        {
            ItemConfig item = itemConfigs.Get(i);
            if (item.category == selectedCategory)
            {
                Widget itemWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BuyMenuItemUI.layout", m_GridSpacerWidget);
                TextWidget itemTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("ItemText"));
                TextWidget priceTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("PriceText"));
                Widget blackMarketWidget = Widget.Cast(itemWidget.FindAnyWidget("BlackMarket"));

                string displayName = ItemNameHelper.GetDisplayName(item.classname);
                itemTextWidget.SetText(displayName);
                priceTextWidget.SetText("$" + FormatCurrency(item.price));

                AddItemPreview(item.classname, itemWidget);

                if (m_IsInSafezone && item.blackmarket)
                { 
                    blackMarketWidget.Show(true); 
                }

                if (!(m_IsInSafezone && item.blackmarket) && item.hasrelated > 0)
                {
                    if (item.hasrelated == 1 || item.hasrelated == 2)
                    {
                        ButtonWidget relatedButton = ButtonWidget.Cast(itemWidget.FindAnyWidget("RelatedButton"));
                        relatedButton.Show(true);
                    }
                    if (item.hasrelated == 1)
                    {
                        Widget magImage = itemWidget.FindAnyWidget("ImageWidgetMag");
                        magImage.Show(true);
                    }
                    else if (item.hasrelated == 2)
                    {
                        Widget ammoImage = itemWidget.FindAnyWidget("ImageWidgetAmmo");
                        ammoImage.Show(true);
                    }
                }

                m_WidgetItemList.Insert(itemWidget, i);
            }
        }
    }

    void AddItemPreview(string itemType, Widget itemWidget)
    {
        if (m_ItemPreviews && m_ItemPreviews.Contains(itemWidget))
        {
            if (m_ItemPreviews[itemWidget].previewEntity)
            { GetGame().ObjectDelete(m_ItemPreviews[itemWidget].previewEntity); }
        }

        ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(itemWidget.FindAnyWidget("ItemPreview"));
        if (!previewWidget)
            return;

        EntityAI previewItem = EntityAI.Cast(GetGame().CreateObject(itemType, "0 0 0", true, false, true));
        if (!previewItem)
            return;

        previewWidget.SetItem(previewItem);
        previewWidget.SetModelPosition(Vector(0, 0, 0.5));
        previewWidget.SetModelOrientation(vector.Zero);
        previewWidget.SetView(previewItem.GetViewIndex());

        m_ItemPreviews.Insert(itemWidget, new PreviewData(previewWidget, previewItem));
    }

    void AddItemToCart(int itemIndex)
    {
        int currentTotalItems = GetTotalCartItems();
        if (currentTotalItems >= CART_ITEM_LIMIT)
        {
            Print("Cart is full. Max " + CART_ITEM_LIMIT + " items per purchase");
            return;
        }

        bool itemExists = false;
        Widget existingWidget;
        foreach (Widget widgetInCart, int cartItemIndex : m_WidgetCartList)
        {
            if (cartItemIndex == itemIndex)
            {
                itemExists = true;
                existingWidget = widgetInCart;
                break;
            }
        }

        string displayName = ItemNameHelper.GetDisplayName(itemConfigs[itemIndex].classname);
        int stackSize = 1;
        bool isAmmo = itemConfigs[itemIndex].hasrelated == 6;
        if (isAmmo) { stackSize = itemConfigs[itemIndex].related.ToInt(); }

        if (itemExists)
        {
            int currentQty = m_ItemQuantities.Get(itemIndex);
            currentQty++;
            m_ItemQuantities.Set(itemIndex, currentQty);

            int totalAmmo = currentQty * stackSize;
            TextWidget cartItemNameWidget = TextWidget.Cast(existingWidget.FindAnyWidget("CartItemText"));

            if (isAmmo || currentQty > 1)
            {
                if (isAmmo) { cartItemNameWidget.SetText(displayName + " [x" + totalAmmo.ToString() + "]"); }
                else { cartItemNameWidget.SetText(displayName + " [x" + currentQty.ToString() + "]"); }
            }
            else { cartItemNameWidget.SetText(displayName); }
        }
        else
        {
            Widget newCartWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BuyMenuCartUI.layout", m_GridSpacerWidgetCart);
            TextWidget cartjtemNameWidget = TextWidget.Cast(newCartWidget.FindAnyWidget("CartItemText"));

            if (isAmmo) { cartjtemNameWidget.SetText(displayName + " [x" + stackSize.ToString() + "]"); }
            else { cartjtemNameWidget.SetText(displayName); }

            m_WidgetCartList.Insert(newCartWidget, itemIndex);
            m_ItemQuantities.Insert(itemIndex, 1);
        }

        UpdateCartTotal();
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        int categoryIndex = w.GetUserID();
        if (categoryIndex >= 0 && categoryIndex < uniqueCategories.Count())
        {
            SetCategory(categoryIndex);
            return true;
        }

        if (w.GetUserID() == ID_AddCart)
        {
            foreach (Widget widget, int itemIndex : m_WidgetItemList)
            {
                if (w == widget || w.GetParent() == widget)
                {
                    AddItemToCart(itemIndex);
                    return true;
                }
            }
        }

        if (w.GetUserID() == ID_AddRelated)
        {
            foreach (Widget relwidget, int relatedItemIndex : m_WidgetItemList)
            {
                if (w == relwidget || w.GetParent() == relwidget)
                {
                    string relatedItemClassname = itemConfigs[relatedItemIndex].related;
                    int relatedIndex = GetRelatedItemIndex(relatedItemClassname);
                    if (relatedIndex != -1)
                    { 
                        AddItemToCart(relatedIndex); 
                    }
                    return true;
                }
            }
        }

        if (w.GetUserID() == ID_RemoveCart)
        {
            int jtemIndex = m_WidgetCartList.Get(w.GetParent());
            int currentQty = m_ItemQuantities.Get(jtemIndex);
            string displayName = ItemNameHelper.GetDisplayName(itemConfigs[jtemIndex].classname);

            int stackSize = 1;
            bool isAmmo = itemConfigs[jtemIndex].hasrelated == 6;
            if (isAmmo)
            { stackSize = itemConfigs[jtemIndex].related.ToInt(); }

            if (currentQty > 1)
            {
                currentQty--;
                m_ItemQuantities.Set(jtemIndex, currentQty);

                int totalAmmo = currentQty * stackSize;
                TextWidget cartItemNameWidget = TextWidget.Cast(w.GetParent().FindAnyWidget("CartItemText"));

                if (isAmmo || currentQty > 1)
                {
                    if (isAmmo)
                    { cartItemNameWidget.SetText(displayName + " [x" + totalAmmo.ToString() + "]"); }
                    else { cartItemNameWidget.SetText(displayName + " [x" + currentQty.ToString() + "]"); }
                }
                else { cartItemNameWidget.SetText(displayName); }
            }
            else
            {
                w.GetParent().Unlink();
                m_WidgetCartList.Remove(w.GetParent());
                m_ItemQuantities.Remove(jtemIndex);
            }

            UpdateCartTotal();
            return true;
        }

        if (w.GetUserID() == ID_Purchase)
        {
            int totalCost = m_CartTotalInt;
            if (totalCost > m_CurrencyAmountInt) 
            {
                Print("Not enough currency to complete the purchase");
                return false;
            }

            if (m_ItemQuantities.Count() > 0)
            {
                string orderDetails = FormatOrderData();
                Param1<string> param = new Param1<string>(orderDetails);
                GetRPCManager().SendRPC("BuyMenu", "BuyMenuTransaction", param);
                ClearCart();
                UpdateCartTotal();
                return true;
            }
            else { return false; }
        }

        if (w.GetUserID() == ID_SubMenu)
        {
            MenuHide();
            m_TransferMenu = new TransferMenuUI();
            m_TransferMenu.ShowTransfer();

            return true;
        }

        if (w.GetUserID() == ID_CloseMenu)
        {
            MenuHide();
            return true;
        }

        return false;
    }

    // UTILITY

    void SetCurrency(int amount)
    {
        m_CurrencyAmountInt = amount;
        if (m_CurrencyAmount) 
        { 
            m_CurrencyAmount.SetText("$" + FormatCurrency(amount)); 
        }
    }

    void SetIsInSafezone(bool isInSafezone)
    { 
        m_IsInSafezone = isInSafezone;
        ClearItemsFromMenu();
        ClearCart();
        PopulateCategories();
    }

    void ClearItemsFromMenu()
    {
        Widget child = m_GridSpacerWidget.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }
        m_WidgetItemList.Clear();
    }

    void ClearCart() 
    {
        Widget child = m_GridSpacerWidgetCart.GetChildren();
        while (child) 
        {
            Widget nextChild = child.GetSibling();
            if (m_ItemPreviews.Contains(child))
            {
                PreviewData previewData = m_ItemPreviews.Get(child);
                if (previewData.previewEntity)
                { GetGame().ObjectDelete(previewData.previewEntity); }
                m_ItemPreviews.Remove(child);
            }
            child.Unlink();
            child = nextChild;
        }

        m_ItemQuantities.Clear();
        m_WidgetCartList.Clear();
    }

    void UpdateCartTotal()
    {
        m_CartTotalInt = GetCartTotal();

        if (m_CartTotalInt > 0)
        {
            m_CartTotal.SetText("$" + FormatCurrency(m_CartTotalInt));
        }
        else
        {
            m_CartTotal.SetText("");
        }
    }

    int GetCartTotal()
    {
        int total = 0;
        foreach (int itemIndex, int qty : m_ItemQuantities)
        {
            total += itemConfigs[itemIndex].price * qty;
        }
        return total;
    }

    int GetTotalCartItems()
    {
        int totalItems = 0;
        foreach (int qty : m_ItemQuantities)
        {
            totalItems += qty;
        }
        return totalItems;
    }

    int GetRelatedItemIndex(string relatedClassname)
    {
        for (int i = 0; i < itemConfigs.Count(); i++)
        {
            if (itemConfigs.Get(i).classname == relatedClassname)
            { return i; }
        }
        return -1;
    }

    string FormatCurrency(int amount)
    {
        string amountStr = amount.ToString();
        string formatted = "";

        int count = 0;
        for (int i = amountStr.Length() - 1; i >= 0; i--)
        {
            formatted = amountStr[i] + formatted;
            count++;
            if (count % 3 == 0 && i != 0)
            {
                formatted = "," + formatted;
            }
        }

        return formatted;
    }

    string FormatOrderData() 
    {
        string order = "";
        foreach (int itemIndex, int quantity : m_ItemQuantities) 
        { order += itemIndex.ToString() + ";" + quantity.ToString() + " "; }
        order = order.Trim();
        return order;
    }

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{		
		if (IsFocusable(w))
		{
			ColorNormal(w);
			return true;
		}
		return false;
	}

    bool IsFocusable(Widget w)
    {
        if (w.GetUserID() == ID_AddCart || w.GetUserID() == ID_AddRelated || w.GetUserID() == ID_RemoveCart || w.GetUserID() == ID_SubMenu || w.GetUserID() == ID_Purchase || w.GetUserID() == ID_CloseMenu)
        { return true; }

        return false;
    }

    void ColorNormal(Widget w)
    {
        if (!w)
			return;

        SetFocus(null);
    }

    // TOGGLE

    void MenuShow()
    {
        if (m_BuyMenu && !m_BuyMenu.IsVisible())
        {
            SetCurrency(m_CurrencyAmountInt);
            GetGame().GetUIManager().ShowScriptedMenu(this, NULL);
            GetGame().GetInput().ChangeGameFocus(1);
            GetGame().GetUIManager().ShowUICursor(true);
            GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
            PPEffects.SetBlurMenu(0.3);
            OnShow();
        }
    }

    void MenuHide()
    {
        if (m_BuyMenu && m_BuyMenu.IsVisible())
        {
            m_BuyMenu.Show(false);
            GetGame().GetUIManager().HideScriptedMenu(this);
            GetGame().GetInput().ResetGameFocus();
            GetGame().GetUIManager().ShowUICursor(false);
            GetGame().GetMission().PlayerControlEnable(false);
            PPEffects.SetBlurMenu(0);
            OnHide();
        }
    }

    bool IsMenuVisible()
    { return m_BuyMenu && m_BuyMenu.IsVisible(); }

    // INSTANCE

    static BuyMenuUI GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new BuyMenuUI();
            m_Instance.Init(); 
        }

        return m_Instance;
    }

    static void ClearInstance()
    { m_Instance = null; }
}