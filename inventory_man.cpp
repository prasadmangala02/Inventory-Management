#include<iostream>
#include<string>
#include<sstream>
#include<ctime>
#include<vector>
#include<algorithm>
//enum status{placed, shipped, delivered};
// TODO: Make separate header files and different cpp files for each of the classes
using namespace std;
// Category class
class Category{
	private:
		int id;
		string name;
		string desc;
	public: 
		// Constructors
	    Category(int id, string name, string desc){
                    id = id;
                    name = name;
                    desc = desc;
            }
	    // Getter and setter
	    int getId(){
		    return id;
	    }
	    string getName(){
		    return name;
	    }
	    string getDesc(){
		    return desc;
	    }
	    void setName(const string& name){
		    this->name = name;
	    }
	    void setDecription(const string& desc){
		    this->desc = desc;
	    }
};

// Product class
class Product{

    private:
	    int id;
	    string name;
	    string desc;
	    float price;
	    int stock_level;
	    int min_reorder_level;
	    Category* category;
    public:
	    Product(int id, string& name, string& desc, float price, int stock_level, int min_order_level, Category* category){
		    id = id;
		    name = name;
		    price = price;
		    stock_level = stock_level;
		    min_order_level = minimum_order_level;
		    category = category;
	    }
	    int getId() const{
		    return id;
	    }
	    string getName() const{
		    return name;
	    }
	    float getPrice() const{
		    return price;
	    }
	    int getStockLevel() const{
		    return stock_level;
	    }
	    int getMinReorderLevel() const{
		    return min_reorder_level;
	    }
	    Category* getCategory(){
		    return category;
	    }
	    void setName(string& name){
		    this->name = name;
	    }
	    void setDesc(string& desc){
		    this->desc = desc;
	    }
	    void setPrice(float price){
		    this->price = price;
	    }
	    void setStockLevel(int stock_level){
		    this->stock_level = stock_level;
	    }
	    void setCategory(Category* category){
		    this->category = category;
	    }
	    void setMinOrderLevel(int min_order_level){
		    this->min_order_level = min_order_level;
	    }
	    bool checkStock(int qty){
		    if(stock_level >= qty){
			    return true;
		    }
		    else return false;
	    }
	    void updateStock(int qty){
		    stock_level -= qty;
	    }

};

// struct representing one order
struct OrderItem{
	Product* product;
	int quantity;
	OrderItem(Product* product, int quantity){
		product = product;
		quantity = quantity;
	}
};
// Order class
class Order{
	private:
		int id;
		string orderDate; // DD-MM-YYYY
		optional<int> customerId; // optional field for customerid
		float total_price;
		string status; // Placed, shipped, delivered
		vector<OrderItem> items;
	public:
		// Constructor
		Order(int id, string& date, string& status) : id(id), orderDate(orderDate), status(status){}

		// Getters and Setters
		int getId() const{
			return id;
	       	}

		optional<int> getCustomerId() const{
			return customerId;
		}
	        float getTotalPrice() const{
			return total_price;
		}
		string getStatus() const{
			return status;
		}
		const vector<orderItem> getOrderItems() const{
			return orderItems;
		}
		
		// Calculates the total price of the order
		void calculateTotal(){
			totalPrice = 0;
			for(auto item:items){
				totalPrice += item.product->getPrice() * item.quantity;
			}
		}
		void processOrder(){
			// If order is not placed, then we cannot process the order.
			if(status != "placed"){
				return;
			}
			bool hasStock = true;
			for(auto item : items){
				if(item.product->checkStock(item.quantity) == false){
					hasStock = false;
					break;
				}
			}

			if(!hasStock){
				// Insufficient stock, mark order as out-of-stock
				status = "out-of-stock"
				cout << "Order " << id << "cannot be processed due to : " << status << endl;
			}

			for(auto item : items){
				item.product->updateStock(item.quantity);
			}
			status = "shipped";
			cout << "Order " << id << " has been shipped : " << status << endl;

		}
		
		
};

// Inventory manager class

class InventoryManager(){
	private:
		vector<Product*> productList;
		unordered_map<int, vector<Product*>> umap; // mapping the list of products according to category id
	
        public:
		void addProduct(Product* product){
			// Push the product into the productList
			productList.push_back(product);
			// If the category for the product exists
			if(product->getCategory()){
				umap[product->getCategory()->getId()].push_back(product);
			}
		
		}
		void removeProduct(int productID){
			// Use an iterator to find if the product is present in the list or not
			auto it = find_if(productList.begin(), productList.end(), [productID](Product* product){return product->getId() == productID;});
			if(it != productList.end()){
				Product* productToRemove = *it;
				productList.erase(it);
				// Remove the product from the category map if it exists
				if(productToRemove->getCategory()){			
					int CategoryIdForRemoval = it->getCategory()->getId();
					// The remove function shifts all the elements that matches to the productToRemove
					//  to the end and returns an iterator pointing to the end. the umap[CategoryIdForRemoval].end()
					//  keeps the iterator that points to the last element in the original array.
					//  So erase function here erases all the elements that has started from the remove-end to the
					//  original-end.
					umap[CategoryIdForRemoval].erase(
							remove(umap[CategoryIdForRemoval].begin(), umap[CategoryIdForRemoval.end(), productToRemove]), umap[CategoryIdForRemoval].end());
				}

				delete productToRemove;
				
			}
			else{
				cout << "ProductID: " << productID << "is not avaliable" <<endl;
			}
		

		}

		Product* getProduct(int productID){
			for(auto product : productList){
				if(product->getId() == productID){
					return product;
				}
			}

			return NULL;
		}
		vector<Product*> getProductByCategory(categoryID){
			if(umap.count(categoryID)){
				return umap[categoryID];
			}
			else return vector<Product*>();
		}
		void placeOrder(Order* order){
			order.processOrder();
			vector<orderItem>* orderedItems = order->getOrderItems();
			// For each item in the orderedItems update the product quantity
			for(auto item : orderedItems){
				item->product->updateStock(item->quantity);
			}
		}
		
		
};

int main(){

	Category category1(1, "Electronics", "Electronic devices");
 	Category category2(2, "Clothing", "Apparel and accessories");
	//Product(int id, string& name, string& desc, float price, int stock_level, int min_order_level, Category* category){
    	Product product1(101, "Laptop", "A powerful laptop for work and play", 899.99, 10, 5, &category1);
    	Product product2(102, "Headphones", "Wireless headphones with noise cancellation", 199.99, 20, 2, &category1);
    	Product product3(103, "T-shirt", "Comfortable cotton T-shirt", 19.99, 50, 10, &category2);
	
	return 0;
}
