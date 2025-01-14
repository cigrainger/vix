defmodule Vix.Vips.ArrayIntTest do
  use ExUnit.Case, async: true

  alias Vix.Vips.ArrayInt

  test "to_nif_term" do
    obj = ArrayInt.to_nif_term([1, 2, 3, 4], nil)

    {:ok, gtype} = Vix.Nif.nif_g_type_from_instance(obj)
    assert Vix.Nif.nif_g_type_name(gtype) == {:ok, "VipsArrayInt"}
  end

  test "to_erl_term" do
    obj = ArrayInt.to_nif_term([1, 2, 3, 4], nil)
    assert [1, 2, 3, 4] == ArrayInt.to_erl_term(obj)
  end
end
