#Marcel Kucharski, 297416
from typing import List
from abc import ABC, abstractmethod
import unittest


class ServerError(Exception):
    def __init__(self, message: str):
        super().__init__(message)


class ProductOverflowError(ServerError):
    def __init__(self, message: str):
        super().__init__(message)


class Product:
    def __init__(self, name: str, price: float):
        self.price = price
        self.name = name


class Server(ABC):
    max_products = 3

    @abstractmethod
    def find_product(self, n: int = 1) -> List[Product]:
        pass

    @abstractmethod
    def search_and_sort(self, products: List[Product], n) -> List[Product]:
        found_products = []
        for x in products:
            if len(x.name) > n and x.name[n - 1].isalpha() and not x.name[n].isalpha():
                found_products.append(x)
        if len(found_products) > self.max_products:
            raise ProductOverflowError("Przekroczono maksymalna ilosc produktow")
        return sorted(found_products, key=lambda product: product.price)


class Client:
    def __init__(self, server: Server):
        self.server = server

    def calculate_value(self, n: int) -> float:
        try:
            found_products = self.server.find_product(n)
            return sum([x.price for x in found_products])
        except ProductOverflowError:
            return 0


class ServerList(Server):
    def __init__(self, products: List[Product]):
        super().__init__()
        self.products = products

    def find_product(self, n: int = 1) -> List[Product]:
        return self.search_and_sort(self.products, n)

    def search_and_sort(self, products: List[Product], n) -> List[Product]:
        temp = super().search_and_sort(self.products, n)
        return temp


class ServerDict(Server):
    def __init__(self, products: List[Product]):
        super().__init__()
        self.products = {}
        for x in products:
            self.products[x.name] = x

    def find_product(self, n: int = 1) -> List[Product]:
        return self.search_and_sort(list(self.products.values()), n)

    def search_and_sort(self, products: List[Product], n):
        temp = super().search_and_sort(products, n)
        return temp


class Tests(unittest.TestCase):
    products = [Product("ABC1", 3.5), Product("EFC2", 1), Product("FSD4", 2)]
    products_err = products + [Product("AVC3", 8)]

    # testowanie sortowania

    def test_sorting_ServerList(self):
        products_sorted = sorted(self.products, key=lambda product: product.price)
        self.assertEqual(ServerList(self.products).find_product(3), products_sorted)

    def test_sorting_ServerDict(self):
        products_sorted = sorted(self.products, key=lambda product: product.price)
        self.assertEqual(ServerDict(self.products).find_product(3), products_sorted)

    # testowanie wyjątku
    def test_list_exception_raise(self):
        with self.assertRaises(ProductOverflowError) as cm:
            ServerList(self.products_err).find_product(3)
        self.assertTrue("Przekroczono maksymalna ilosc produktow" in str(cm.exception))

    def test_dict_exception_raise(self):
        with self.assertRaises(ProductOverflowError) as cm:
            ServerDict(self.products_err).find_product(3)
        self.assertTrue("Przekroczono maksymalna ilosc produktow" in str(cm.exception))

    # testowanie obliczania ceny przy błędzie
    def test_exception_in_calculate_value(self):
        server = ServerList(self.products_err)
        client = Client(server)
        self.assertEqual(client.calculate_value(3), 0)

    # testowanie sumy ceny produktow
    def test_calculating_value(self):
        products = [Product("ABC1", 3.5), Product("EFC2", 1), Product("FSD4", 2)]
        server = ServerList(products)
        client = Client(server)
        self.assertEqual(client.calculate_value(3), 6.5)


if __name__ == '__main__':
    unittest.main()
#Marcel Kucharski, 297416
