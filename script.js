const products = {
  "IQOS ILUMA": 1590,
  "IQOS ILUMA PRIME": 2490,
  "TEREA Silver": 890,
  "TEREA Bronze": 890
};

let cartCount = Number(localStorage.getItem("cartCount")) || 0;

let orders = JSON.parse(localStorage.getItem("orders")) || [
  {
    id: 2451,
    date: "2026-03-12",
    product: "IQOS ILUMA",
    quantity: 1,
    status: "Doručeno",
    price: 1590
  },
  {
    id: 2448,
    date: "2026-03-01",
    product: "TEREA Bronze",
    quantity: 1,
    status: "Odesláno",
    price: 890
  }
];

let editingOrderId = null;

const cartCountElement = document.getElementById("cartCount");
const cartText = document.getElementById("cartText");
const orderTableBody = document.getElementById("orderTableBody");
const searchInput = document.getElementById("searchOrderInput");

const orderForm = document.getElementById("orderForm");
const orderProduct = document.getElementById("orderProduct");
const orderQuantity = document.getElementById("orderQuantity");
const orderPrice = document.getElementById("orderPrice");
const orderStatus = document.getElementById("orderStatus");
const orderModalTitle = document.getElementById("orderModalTitle");

function saveData() {
  localStorage.setItem("orders", JSON.stringify(orders));
  localStorage.setItem("cartCount", cartCount);
}

function generateOrderId() {
  if (orders.length === 0) return 1001;
  return Math.max(...orders.map(order => order.id)) + 1;
}

function updateCart() {
  cartCountElement.textContent = cartCount;
  cartText.textContent =
    cartCount === 0
      ? "Košík je prázdný."
      : `V košíku máte ${cartCount} produktů.`;
}

function calculatePrice() {
  const product = orderProduct.value;
  const quantity = Number(orderQuantity.value) || 1;
  const price = products[product] * quantity;

  orderPrice.value = `${price} Kč`;
  return price;
}

function renderOrders(data = orders) {
  orderTableBody.innerHTML = "";

  if (data.length === 0) {
    orderTableBody.innerHTML = `
      <tr>
        <td colspan="6" class="text-center text-secondary py-4">
          Žádné objednávky nebyly nalezeny
        </td>
      </tr>
    `;
    return;
  }

  data.forEach(order => {
    const statusClass =
      order.status === "Doručeno"
        ? "success"
        : order.status === "Odesláno"
        ? "info"
        : "warning";

    orderTableBody.innerHTML += `
      <tr>
        <td>${order.id}</td>
        <td>${formatDate(order.date)}</td>
        <td>${order.product}</td>
        <td>${order.quantity}</td>
        <td>
          <span class="badge text-bg-${statusClass}">
            ${order.status}
          </span>
        </td>
        <td>${order.price} Kč</td>
        <td>
          <div class="d-flex gap-2">
            <button class="btn btn-sm btn-outline-primary" onclick="editOrder(${order.id})">
              Upravit
            </button>
            <button class="btn btn-sm btn-outline-danger" onclick="deleteOrder(${order.id})">
              Smazat
            </button>
          </div>
        </td>
      </tr>
    `;
  });
}

function formatDate(dateValue) {
  const date = new Date(dateValue);
  return date.toLocaleDateString("cs-CZ");
}

function resetOrderForm() {
  editingOrderId = null;
  orderModalTitle.textContent = "Nová objednávka";
  orderForm.reset();
  orderQuantity.value = 1;
  orderStatus.value = "Nová";
  calculatePrice();
}

function editOrder(id) {
  const order = orders.find(order => order.id === id);
  if (!order) return;

  editingOrderId = id;

  orderModalTitle.textContent = `Upravit objednávku #${id}`;

  orderProduct.value = order.product;
  orderQuantity.value = order.quantity;
  orderStatus.value = order.status;
  orderPrice.value = `${order.price} Kč`;

  const modal = new bootstrap.Modal(document.getElementById("orderModal"));
  modal.show();
}

function deleteOrder(id) {
  const confirmDelete = confirm("Opravdu chcete smazat tuto objednávku?");
  if (!confirmDelete) return;

  orders = orders.filter(order => order.id !== id);

  saveData();
  renderOrders();
}

orderForm.addEventListener("submit", function (event) {
  event.preventDefault();

  const product = orderProduct.value;
  const quantity = Number(orderQuantity.value);
  const status = orderStatus.value;
  const price = calculatePrice();

  if (editingOrderId) {
    orders = orders.map(order => {
      if (order.id === editingOrderId) {
        return {
          ...order,
          product,
          quantity,
          status,
          price
        };
      }

      return order;
    });
  } else {
    const newOrder = {
      id: generateOrderId(),
      date: new Date().toISOString().split("T")[0],
      product,
      quantity,
      status,
      price
    };

    orders.unshift(newOrder);
  }

  saveData();
  renderOrders();
  resetOrderForm();

  const modal = bootstrap.Modal.getInstance(document.getElementById("orderModal"));
  modal.hide();
});

orderProduct.addEventListener("change", calculatePrice);
orderQuantity.addEventListener("input", calculatePrice);

searchInput.addEventListener("input", function () {
  const query = searchInput.value.toLowerCase();

  const filteredOrders = orders.filter(order =>
    String(order.id).includes(query) ||
    order.product.toLowerCase().includes(query) ||
    order.status.toLowerCase().includes(query)
  );

  renderOrders(filteredOrders);
});

document.querySelectorAll(".add-cart").forEach(button => {
  button.addEventListener("click", () => {
    cartCount++;
    saveData();
    updateCart();
  });
});

updateCart();
renderOrders();
calculatePrice();
