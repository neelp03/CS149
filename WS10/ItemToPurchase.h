struct ItemToPurchase {
	char itemName[50];
	int itemPrice;
	int itemQuantity;
	
	void (*makeBlank)(struct ItemToPurchase*);
	void (*print)(struct ItemToPurchase);
};


