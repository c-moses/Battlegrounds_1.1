class PreviewData
{
    ItemPreviewWidget previewWidget;
    EntityAI previewEntity;

    void PreviewData(ItemPreviewWidget widget, EntityAI entity)
    {
        previewWidget = widget;
        previewEntity = entity;
    }
};

class ItemNameHelper
{
    static string GetDisplayName(string classname)
    {
        string displayName = "";
        if (GetGame().ConfigGetText(CFG_VEHICLESPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        if (GetGame().ConfigGetText(CFG_WEAPONSPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        if (GetGame().ConfigGetText(CFG_MAGAZINESPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        return classname;
    }
    
    static string TrimUnt(string input)
    {
        int index = input.IndexOf("$UNT$");
        if (index != -1)
        {
            string firstPart = input.Substring(0, index);
            string secondPart = input.Substring(index + 5, input.Length() - (index + 5));
            input = firstPart + secondPart;
        }
        return input;
    }
};

class ItemConfig
{
    string category;  // weapons, ammo/attach, gear, medical
    string classname; // item classname
    int hasrelated;   // 0 = no related, 1 = mag, 2 = ammo, 3 = pouches, 4 = visor, 5 = battery
    // hasrelated 6 = flag to update loose ammo qty in cart (string related will hold the qty count)
    string related;   // related item classname (magazine or ammo)
    int price;        // price for that [x1] item
    bool blackmarket; // is this item black market only

    void ItemConfig(string cat, string clss, int hasrel, string relate, int prc, bool isBlackMarket)
    {
        category = cat;
        classname = clss;
        hasrelated = hasrel;
        related = relate;
        price = prc;
        blackmarket = isBlackMarket;
    }
};

ref array<ref ItemConfig> itemConfigs = 
{
    // Weapons
    new ItemConfig("Weapons", "MP5K", 1, "Mag_MP5_30Rnd", 500, false),
    new ItemConfig("Weapons", "UMP45", 1, "Mag_UMP_25Rnd", 500, false),
    new ItemConfig("Weapons", "PP19", 1, "Mag_PP19_64Rnd", 600, false),
    new ItemConfig("Weapons", "AKS74U", 1, "Mag_AK74_30Rnd", 700, false),
    new ItemConfig("Weapons", "AK74", 1, "Mag_AK74_30Rnd", 800, false),
    new ItemConfig("Weapons", "Vikhr", 1, "Mag_Vikhr_30Rnd", 900, false),
    new ItemConfig("Weapons", "B95", 2, "Ammo_308Win", 1000, false),
    new ItemConfig("Weapons", "Winchester70", 2, "Ammo_308Win", 1100, false),
    new ItemConfig("Weapons", "Mosin9130", 2, "Ammo_762x54", 1200, false),
    new ItemConfig("Weapons", "SKS", 2, "Ammo_762x39", 1300, false),
    new ItemConfig("Weapons", "AK101", 1, "Mag_AK101_30Rnd", 1400, false),
    new ItemConfig("Weapons", "VSS", 1, "Mag_Vikhr_30Rnd", 1500, false),
    new ItemConfig("Weapons", "ASVAL", 1, "Mag_Vikhr_30Rnd", 1600, false),
    new ItemConfig("Weapons", "Aug", 1, "Mag_STANAG_30Rnd", 1700, false),
    new ItemConfig("Weapons", "AKM", 1, "Mag_AKM_30Rnd", 1800, false),
    new ItemConfig("Weapons", "SawedoffFAMAS", 1, "Mag_FAMAS_25Rnd", 1900, true),
    new ItemConfig("Weapons", "M4A1", 1, "Mag_STANAG_60Rnd", 2000, true),
    new ItemConfig("Weapons", "SVD", 1, "Mag_SVD_10Rnd", 2200, true),
    new ItemConfig("Weapons", "M14", 1, "Mag_M14_20Rnd", 2300, true),
    new ItemConfig("Weapons", "FAL", 1, "Mag_FAL_20Rnd", 2400, true),
    
    // Attachments
    new ItemConfig("Attachments", "KobraOptic", 5, "Battery9V", 200, false),
    new ItemConfig("Attachments", "ReflexOptic", 5, "Battery9V", 400, false),
    new ItemConfig("Attachments", "PSO1Optic", 5, "Battery9V", 800, false),
    new ItemConfig("Attachments", "ACOGOptic", 0, "", 900, false),
    new ItemConfig("Attachments", "HuntingOptic", 0, "", 1000, false),
    new ItemConfig("Attachments", "PUScopeOptic", 0, "", 1000, false),
    new ItemConfig("Attachments", "ACOGOptic_6x", 0, "", 1200, false),
    new ItemConfig("Attachments", "PistolSuppressor", 0, "", 1200, false),
    new ItemConfig("Attachments", "UniversalSuppressorAK", 0, "", 2000, true),
    new ItemConfig("Attachments", "UniversalSuppressorM4", 0, "", 2000, true),

    // Magazines
    new ItemConfig("Magazines", "Mag_MP5_30Rnd", 2, "Ammo_9x19", 50, false),
    new ItemConfig("Magazines", "Mag_UMP_25Rnd", 2, "Ammo_45ACP", 50, false),
    new ItemConfig("Magazines", "Mag_PP19_64Rnd", 2, "Ammo_380", 70, false),
    new ItemConfig("Magazines", "Mag_AK74_30Rnd", 2, "Ammo_545x39", 70, false),
    new ItemConfig("Magazines", "Mag_Vikhr_30Rnd", 2, "Ammo_9x39", 100, false),
    new ItemConfig("Magazines", "Mag_AK101_30Rnd", 2, "Ammo_556x45", 100, false),
    new ItemConfig("Magazines", "Mag_STANAG_30Rnd", 2, "Ammo_556x45", 120, false),
    new ItemConfig("Magazines", "Mag_AKM_30Rnd", 2, "Ammo_762x39", 120, false),
    new ItemConfig("Magazines", "Mag_FAMAS_25Rnd", 2, "Ammo_556x45", 150, true),
    new ItemConfig("Magazines", "Mag_STANAG_60Rnd", 2, "Ammo_556x45", 200, true),
    new ItemConfig("Magazines", "Mag_AKM_Drum75Rnd", 2, "Ammo_762x39", 200, true),
    new ItemConfig("Magazines", "Mag_SVD_10Rnd", 2, "Ammo_762x54", 220, true),
    new ItemConfig("Magazines", "Mag_M14_20Rnd", 2, "Ammo_308Win", 250, true),
    new ItemConfig("Magazines", "Mag_FAL_20Rnd", 2, "Ammo_308Win", 250, true),

    // Ammunition
    new ItemConfig("Ammunition", "Ammo_9x19", 6, "30", 50, false),
    new ItemConfig("Ammunition", "Ammo_45ACP", 6, "30", 50, false),
    new ItemConfig("Ammunition", "Ammo_380", 6, "30", 50, false),
    new ItemConfig("Ammunition", "Ammo_545x39", 6, "20", 70, false),   
    new ItemConfig("Ammunition", "Ammo_9x39", 6, "20", 100, false), 
    new ItemConfig("Ammunition", "Ammo_556x45", 6, "20", 100, false), 
    new ItemConfig("Ammunition", "Ammo_762x39", 6, "20", 100, false),
    new ItemConfig("Ammunition", "Ammo_308Win", 6, "20", 150, false),
    new ItemConfig("Ammunition", "Ammo_762x54", 6, "20", 150, false),

    // Gear
    new ItemConfig("Gear", "GorkaHelmet_Green", 0, "", 500, false),
    new ItemConfig("Gear", "PlateCarrierVest_Green", 3, "PlateCarrierPouches_Green", 800, false),
    new ItemConfig("Gear", "GorkaHelmet_Black", 0, "", 500, false),
    new ItemConfig("Gear", "PlateCarrierVest_Black", 3, "PlateCarrierPouches_Black", 800, false), 
    new ItemConfig("Gear", "MVS_Altyn_OD", 4, "MVS_Altyn_Visor_OD", 1500, true), 
    new ItemConfig("Gear", "MVS_Combat_Vest_Heavy_OD", 3, "MVS_Heavy_Pouch_OD", 2000, true),
    new ItemConfig("Gear", "MVS_Altyn_Black", 4, "MVS_Altyn_Visor_Black", 1500, true), 
    new ItemConfig("Gear", "MVS_Combat_Vest_Heavy_Black", 3, "MVS_Heavy_Pouch_Black", 2000, true),
    new ItemConfig("Gear", "GhillieSuit_Woodland", 0, "", 1200, true),
    new ItemConfig("Gear", "GhillieSuit_Mossy", 0, "", 1200, true),
    new ItemConfig("Gear", "MVS_Rucksack_OD", 0, "", 1200, false),
    new ItemConfig("Gear", "MVS_Rucksack_Black", 0, "", 1200, false),

    // Medical
    new ItemConfig("Medical", "BandageDressing", 0, "", 100, false),
    new ItemConfig("Medical", "AntiChemInjector", 0, "", 100, false),
    new ItemConfig("Medical", "Morphine", 0, "", 100, false),
    new ItemConfig("Medical", "SalineBagIV", 0, "", 200, false),
    new ItemConfig("Medical", "TetracyclineAntibiotics", 0, "", 200, false),
    new ItemConfig("Medical", "Epinephrine", 0, "", 300, false),
    new ItemConfig("Medical", "BloodBagIV", 0, "", 500, false),
    new ItemConfig("Medical", "FirstAidKit_BG", 0, "", 800, false),

    // Other
    new ItemConfig("Other", "Battery9V", 0, "", 100, false),
    new ItemConfig("Other", "GasMask", 0, "", 200, false),
    new ItemConfig("Other", "SewingKit", 0, "", 300, false),
    new ItemConfig("Other", "LeatherSewingKit", 0, "", 400, false),
    new ItemConfig("Other", "WeaponCleaningKit", 0, "", 500, false),
};