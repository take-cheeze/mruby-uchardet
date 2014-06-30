assert 'UTF-8 detect' do
  det = UCharDet.new
  det.handle 'こんにちわ世界'
  det.end
  assert_equal 'UTF-8', det.result
end

assert 'Shift JIS detect' do
  det = UCharDet.new UCharDet::FILTER_ALL
  det.handle "\x82\xb1\x82\xf1\x82\xc9\x82\xbf\x82\xed\x90\xa2\x8a\x45"
  det.end
  assert_equal "Shift_JIS", det.result
end

assert 'result before end' do
  det = UCharDet.new
  assert_raise(RuntimeError) { det.result }
end
